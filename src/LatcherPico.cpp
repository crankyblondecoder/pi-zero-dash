#include <fcntl.h>
#include <iostream>
#include <linux/spi/spidev.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

#include "LatcherPico.hpp"

using namespace piZeroDash;

LatcherPico::~LatcherPico()
{
	if(_spiFd > -1) close(_spiFd);
	if(_gpioChipFd > -1) close(_gpioChipFd);
}

LatcherPico::LatcherPico()
{
	// Setup SPI comms to the pico.

	// Hardwire to first SPI bus with first chip select line.
	_spiFd = open("/dev/spidev0.0", O_RDWR);

	if(_spiFd > -1)
	{
		// @see https://github.com/torvalds/linux/blob/master/include/uapi/linux/spi/spidev.h
		// @see https://docs.kernel.org/spi/spidev.html
		// @see https://docs.kernel.org/driver-api/spi.html

		// Assume the Pico is using Motorola mode (SPO=0, SPH=1).
		//
		// From the RP2040 manual:
		//
		// In the case of a single word transfer, after all bits have been transferred, the SSPFSSOUT line is returned to its idle
		// HIGH state one SSPCLKOUT period after the last bit has been captured. For continuous back-to-back transfers, the
		// SSPFSSOUT pin is held LOW between successive data words and termination is the same as that of the single word transfer.
		//
		// Note: Assume it doesn't require CS to go high in between data words.

		// Use SPI_MODE_1
		static uint8_t mode = SPI_MODE_1;

		int error = ioctl(_spiFd, SPI_IOC_WR_MODE, &mode);

		if(error > -1)
		{
			// Bits per frame.
			static uint8_t bits = 8;

    		error = ioctl(_spiFd, SPI_IOC_WR_BITS_PER_WORD, &bits);

			if(error > -1)
			{
				// Set SPI clock freq to something Pico can handle. Assume the max setup on the Pico is 8Mhz and go lower
				// than that. Set quite low so that a cheap logic analyser can get enough samples.
				uint32_t clockHz = 4 * 1000 * 1000;
				error = ioctl(_spiFd, SPI_IOC_WR_MAX_SPEED_HZ, &clockHz);
				if(error == -1) cout << "SPI Bus Clock ioctl error: " << strerror(errno) << "\n";

				// Output SPI clock speed info.
				error = ioctl(_spiFd, SPI_IOC_RD_MAX_SPEED_HZ, &clockHz);
				if(error > -1) cout << "SPI Bus Clock " << clockHz/1000 << "khz.\n";

				// Attempt to open the GPIO device.
				_gpioChipFd = open("/dev/gpiochip0", O_RDWR);

				if(_gpioChipFd > -1)
				{
					// Get chip line that matches required GPIO.
					gpio_v2_line_request lineReq = {};

					lineReq.offsets[0] = PZD_MASTER_ACTIVE_GPIO;

					lineReq.consumer[0] = 'P';
					lineReq.consumer[1] = 'Z';
					lineReq.consumer[2] = 'D';
					lineReq.consumer[3] = 'A';
					lineReq.consumer[4] = 'S';
					lineReq.consumer[5] = 'H';
					lineReq.consumer[6] = 0;

					// Default flags.
					lineReq.config.flags = GPIO_V2_LINE_FLAG_OUTPUT;

					// No line specific attributes are necessary.
					lineReq.config.num_attrs = 0;

					lineReq.config.padding[0] = 0;
					lineReq.config.padding[1] = 0;
					lineReq.config.padding[2] = 0;
					lineReq.config.padding[3] = 0;
					lineReq.config.padding[4] = 0;

					lineReq.num_lines = 1;
					lineReq.event_buffer_size = 0;

					lineReq.padding[0] = 0;
					lineReq.padding[1] = 0;
					lineReq.padding[2] = 0;
					lineReq.padding[3] = 0;
					lineReq.padding[4] = 0;

					lineReq.fd = 0;

					error = ioctl(_gpioChipFd, GPIO_V2_GET_LINE_IOCTL, &lineReq);

					if(error > -1)
					{
						_gpioLineFd = lineReq.fd;

						// Assume ready has been initialised to false prior.
						_ready = true;

						// Make sure master active is in known "off" state.
						__setMasterActive(false);

						// Can now attempt to initialise latched data indexes and resolutions.
						__downloadLatchedDataIndexes();
						__downloadLatchedDataResolutions();
					}
					else
					{
						cout << "GPIO line request failed via ioctl.\n";
					}
				}
				else
				{
					cout << "Could not open GPIO device file.\n";
				}
			}
			else
			{
				cout << "Could not set SPI bits per word via ioctl.\n";
			}
		}
		else
		{
			cout << "Could not set SPI mode via ioctl.\n";
		}
	}
	else
	{
		cout << "Could not open SPI device file.\n";
	}
}

void LatcherPico::_poll()
{
	if(_ready)
	{
		__setMasterActive(true);

		// TODO ...

		__setMasterActive(false);
	}
}

bool LatcherPico::_isReady()
{
	return _ready;
}

void LatcherPico::__setMasterActive(bool masterActive)
{
	if(_gpioChipFd > -1 && _gpioLineFd > -1)
	{
		gpio_v2_line_values lineVals;

		// Matches index into lineReq.offsets array when line was requested.
		lineVals.mask = 1;

		if(masterActive)
		{
			// Take the "master active" GPIO high to indicate to the pico that comms is active.
			// Matches index into lineReq.offsets array when line was requested.
			lineVals.bits = 1;
		}
		else
		{
			// Take the "master active" GPIO low to indicate to the pico that comms is deactive.
			// Matches index into lineReq.offsets array when line was requested.
			lineVals.bits = 0;
		}

		// Yes the _line_ file descriptor is used, not the GPIO device file descriptor.
		int error = ioctl(_gpioLineFd, GPIO_V2_LINE_SET_VALUES_IOCTL, &lineVals);

		if(error != 0)
		{
			cout << "Error while setting master active to:" << masterActive << " errno:" << errno << "\n";
		}
	}
}

bool LatcherPico::
__picoSpiTxRx(uint8_t* txBuf, uint8_t* rxBuf, int length, uint8_t reqId)
{
	// Single master active/deactive cycle per SPI transfer session. This hopefully should give the transfers a
	// "reset on fault" tolerance because it is assumed the pico resets its SPI transfer when master active goes low.
	__setMasterActive(true);

	// Rely on the pico sending data contiguously. So once the request id is received assume all expected data follows.
	// ie The pico will fill its TX FIFO before enabling the send so there will be no gaps in the byte stream.

	bool success = true;

	if(_spiFd > -1)
	{
		int bytesToRead = length;

		// Offset to add to buffers for partial reads.
		unsigned long bufOffset = 0;

		while(bytesToRead)
		{
			// Clear the rx buffer to zero.
			for(int index = 0; index < length; index++)
			{
				rxBuf[index] = 0;
			}

			// Create, setup and where necessary zero an SPI transfer structure.
			// See: https://codebrowser.dev/linux/linux/include/uapi/linux/spi/spidev.h.html

			spi_ioc_transfer transf;

			transf.tx_buf = (unsigned long) txBuf;
			transf.rx_buf = (unsigned long) (rxBuf + bufOffset);
			transf.len = bytesToRead;
			transf.speed_hz = 0; // Use default.
			transf.delay_usecs = 0;
			transf.bits_per_word = 0; // Use default.
			transf.cs_change = true;
			transf.tx_nbits = 0;
			transf.rx_nbits = 0;
			transf.word_delay_usecs = 0;
			transf.pad = 0;

			int error = ioctl(_spiFd, SPI_IOC_MESSAGE(1), &transf);

			if(error >= 0)
			{
				// Transmission okay.

				if(!reqId || rxBuf[0] == reqId)
				{
					// Don't have to wait for the request id so done.
					bytesToRead = 0;
				}
				else
				{
					// Look for the request id in the rx buffer.
					// This should only ever happen once.
					// RX data should be 0's prior to the request id being found.

					// If the first non-zero value found is not the request id then abort the transfer

					int index;
					bool badReqId = false;

					for(index = 0; index < length; index++)
					{
						if(rxBuf[index] == reqId)
						{
							break;
						}
						else if(rxBuf[index] > 0)
						{
							// Bad request id found. Abort transfer.
							badReqId = true;
							bytesToRead = 0;
							success = false;
							break;
						}
					}

					if(!badReqId && index < length)
					{
						// Request id was found.

						// Setup for the final iteration of the transfer.
						bytesToRead = index;
						bufOffset = length - bytesToRead;

						// Shift all read buffer entries left until the request id is the first entry.
						for(int shiftToIndex = 0; index < length; index++, shiftToIndex++)
						{
							rxBuf[shiftToIndex] = rxBuf[index];
						}

						// Set the entire transmit buffer to zero as it will have already been sent.
						for(index = 0; index < length; index++) txBuf[index] = 0;
					}

					// Note: If the request id was not found the number of bytes to read should still be the full amount.
				}
			}
			else
			{
				// Transmission not okay. Abort.
				bytesToRead = 0;
				success = false;
 			}
		}
	}

	__setMasterActive(false);

	return success;
}

int LatcherPico::__downloadLatchedDataIndex(const char* latchedDataIndexName)
{
	int retVal = -1;

	// Note: Don't use the same request id and command. It makes it difficult to debug on the pico.

	_txBuf[0] = GET_LATCHED_DATA_INDEX;
	_txBuf[1] = GET_LATCHED_DATA_INDEX_REQ_ID;

	_txBuf[2] = latchedDataIndexName[0];
	_txBuf[3] = latchedDataIndexName[1];
	_txBuf[4] = latchedDataIndexName[2];

	bool okay = __picoSpiTxRx(_txBuf, _rxBuf, 5, 0);

	if(okay)
	{
		okay = __picoSpiTxRx(_txBuf, _rxBuf, 2, GET_LATCHED_DATA_INDEX_REQ_ID);

		if(okay)
		{
			retVal = _rxBuf[1];
		}
	}

	return retVal;
}

void LatcherPico::__downloadLatchedDataIndexes()
{
	// Data index strings are from pico_dash_latch.c

	cout << "Waiting for ERM latched data index.\n";

	// Engine RPM.
	_picoLatchedDataIndexes[LatcherPico::ENGINE_RPM] = __downloadLatchedDataIndex("ERM");

	cout << "ERM Index: "  << _picoLatchedDataIndexes[LatcherPico::ENGINE_RPM] << "\n";

	cout << "Waiting for KMH latched data index.\n";

	// Speed in KMH.
	_picoLatchedDataIndexes[LatcherPico::SPEED_KMH] = __downloadLatchedDataIndex("SKH");

	cout << "SKH Index: "  << _picoLatchedDataIndexes[LatcherPico::SPEED_KMH] << "\n";

	cout << "Waiting for ETC latched data index.\n";

	// Engine temperature degrees celsius.
	_picoLatchedDataIndexes[LatcherPico::ENGINE_TEMP_C] = __downloadLatchedDataIndex("ETC");

	cout << "ETC Index: "  << _picoLatchedDataIndexes[LatcherPico::ENGINE_TEMP_C] << "\n";
}

void LatcherPico::__downloadLatchedDataResolutions()
{
	// TODO ...
}
