#include <fcntl.h>
#include <iostream>
#include <linux/spi/spidev.h>
#include <linux/gpio.h>
#include <poll.h>
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
	// Clear latched data index data.
	for(int index = 0; index < MAX_LATCHED_INDEXES; index++) _picoLatchedDataIndexes[index] = 0;

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
					// Get chip lines that matches required GPIO's.
					gpio_v2_line_request lineReq = {};

					lineReq.offsets[0] = PZD_COMMAND_ACTIVE_GPIO;
					lineReq.offsets[1] = PZD_READY_FOR_COMMAND_GPIO;

					lineReq.consumer[0] = 'P';
					lineReq.consumer[1] = 'Z';
					lineReq.consumer[2] = 'D';
					lineReq.consumer[3] = 'A';
					lineReq.consumer[4] = 'S';
					lineReq.consumer[5] = 'H';
					lineReq.consumer[6] = 0;

					// Default flags.
					lineReq.config.flags = 0;

					// Because more than one GPIO, line specific attributes are necessary.
					lineReq.config.num_attrs = 2;

					lineReq.config.attrs[0].attr.id = GPIO_V2_LINE_ATTR_ID_FLAGS;
					lineReq.config.attrs[0].attr.padding = 0;
					lineReq.config.attrs[0].attr.flags = GPIO_V2_LINE_FLAG_OUTPUT;
					lineReq.config.attrs[0].mask = 1;

					lineReq.config.attrs[1].attr.id = GPIO_V2_LINE_ATTR_ID_FLAGS;
					lineReq.config.attrs[1].attr.padding = 0;
					lineReq.config.attrs[1].attr.flags = GPIO_V2_LINE_FLAG_INPUT | GPIO_V2_LINE_FLAG_EDGE_RISING |
						GPIO_V2_LINE_FLAG_EDGE_FALLING;
					lineReq.config.attrs[1].mask = 2;

					lineReq.config.padding[0] = 0;
					lineReq.config.padding[1] = 0;
					lineReq.config.padding[2] = 0;
					lineReq.config.padding[3] = 0;
					lineReq.config.padding[4] = 0;

					lineReq.num_lines = 2;
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

						// Make sure master active is in known "off" state.
						__setCommandActive(false);

						// Can now attempt to initialise latched data indexes and resolutions.
						__downloadLatchedDataIndexes();
						__downloadLatchedDataResolutions();

						// Can signal to polling that it can start. Assume ready has been initialised to false prior.
						_ready = true;
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
		__downloadLatchedDataRaw();
	}
}

bool LatcherPico::_isReady()
{
	return _ready;
}

void LatcherPico::__setCommandActive(bool commandActive)
{
	if(_gpioChipFd > -1 && _gpioLineFd > -1)
	{
		gpio_v2_line_values lineVals;

		// Matches index into lineReq.offsets array when line was requested.
		lineVals.mask = 1;

		if(commandActive)
		{
			// Take the "command active" GPIO high to indicate to the pico that comms is active.
			// Matches index into lineReq.offsets array when line was requested.
			lineVals.bits = 1;
		}
		else
		{
			// Take the "command active" GPIO low to indicate to the pico that comms is deactive.
			// Matches index into lineReq.offsets array when line was requested.
			lineVals.bits = 0;
		}

		// Yes the _line_ file descriptor is used, not the GPIO device file descriptor.
		int error = ioctl(_gpioLineFd, GPIO_V2_LINE_SET_VALUES_IOCTL, &lineVals);

		if(error < 0)
		{
			cout << "Error while setting master active to:" << commandActive << " errno:" << errno << "\n";
		}
	}
}

int LatcherPico::__getReadyForCommandActive()
{
	int retVal = -1;

	if(_gpioLineFd > -1)
	{
		gpio_v2_line_values lineVals;
		lineVals.mask = 2;

		int error = ioctl(_gpioLineFd, GPIO_V2_LINE_GET_VALUES_IOCTL, &lineVals);

		if(error > -1)
		{
			retVal = (lineVals.bits & 2) >> 1;
		}
	}

	return retVal;
}

int LatcherPico::__readGpioEventsBlocking()
{
	int retVal = -1;

	if(_gpioLineFd > -1)
	{
		// Note: Should read the entire event buffer in one go.
		retVal = read(_gpioLineFd, &_gpioLineEventBuffer, sizeof(gpio_v2_line_event) * GPIO_LINE_EVENT_BUFFER_SIZE);

		if(retVal > 0) retVal /= sizeof(gpio_v2_line_event);
	}

	return retVal;
}

int LatcherPico::__readGpioEventsNonBlocking(int timeout)
{
	// Poll GPIO line FD so reading it doesn't block.

	int retVal = 0;

	if(_gpioLineFd > -1)
	{
		pollfd pollFd;
		pollFd.fd = _gpioLineFd;
		pollFd.events = POLLIN;
		pollFd.revents = 0;

		int numPollEvents = poll(&pollFd, 1, timeout);

		if(numPollEvents > 0)
		{
			// Shouldn't block.
			retVal = __readGpioEventsBlocking();
		}
	}

	return retVal;
}

void LatcherPico::__clearGpioEvents()
{
	__readGpioEventsNonBlocking(0);
}

bool LatcherPico::__waitForReadyForCommandActive()
{
	int readyForCommandActive = __getReadyForCommandActive();

	bool error = readyForCommandActive == -1;
	bool edgeHighFound = false;

	while(readyForCommandActive == 0 && !edgeHighFound)
	{
		int numEvents = __readGpioEventsNonBlocking(WAIT_FOR_READY_TIMEOUT);

		if(numEvents <= 0)
		{
			error = true;
			break;
		}

		// Detect ready for command going high in the list of events. Even if ready for command is currently inactive,
		// ready for command going active at any time in pending events causes the wait to stop. This mitigates a
		// deadlock when the Pico rapidly flips from inactive to active to inactive.

		for(int index = 0; index < numEvents; index++)
		{
			if(_gpioLineEventBuffer[index].id == GPIO_V2_LINE_EVENT_RISING_EDGE)
			{
				edgeHighFound = true;
				break;
			}
		}

		readyForCommandActive = __getReadyForCommandActive();

		if(readyForCommandActive == -1)
		{
			error = true;
			break;
		}
	}

	return !error && readyForCommandActive == 1;
}

bool LatcherPico::__waitForReadyForCommandInactive()
{
	int readyForCommandActive = __getReadyForCommandActive();

	bool error = readyForCommandActive == -1;
	bool edgeLowFound = false;

	while(readyForCommandActive == 1 && !edgeLowFound)
	{
		int numEvents = __readGpioEventsNonBlocking(WAIT_FOR_READY_TIMEOUT);

		if(numEvents <= 0)
		{
			error = true;
			break;
		}

		// Detect ready for command going high in the list of events. Even if ready for command is currently inactive,
		// ready for command going active at any time in pending events causes the wait to stop. This mitigates a
		// deadlock when the Pico rapidly flips from inactive to active to inactive.

		for(int index = 0; index < numEvents; index++)
		{
			if(_gpioLineEventBuffer[index].id == GPIO_V2_LINE_EVENT_FALLING_EDGE)
			{
				edgeLowFound = true;
				break;
			}
		}

		readyForCommandActive = __getReadyForCommandActive();

		if(readyForCommandActive == -1)
		{
			error = true;
			break;
		}
	}

	return !error && readyForCommandActive == 0;
}

bool LatcherPico::__picoSpiTxRx(uint8_t* txBuf, uint8_t* rxBuf, int length)
{
	bool success = false;

	if(_spiFd > -1)
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
			transf.rx_buf = (unsigned long) rxBuf;
			transf.len = length;
			transf.speed_hz = 0; // Use default.
			transf.delay_usecs = 0;
			transf.bits_per_word = 0; // Use default.
			transf.cs_change = true;
			transf.tx_nbits = 0;
			transf.rx_nbits = 0;
			transf.word_delay_usecs = 0;
			transf.pad = 0;

			int error = ioctl(_spiFd, SPI_IOC_MESSAGE(1), &transf);

			success = error >= 0;
	}

	return success;
}

bool LatcherPico::__sendRecvCommand()
{
	// Assumes command is in the tx buffer and reply is in the rx buffer and that the command is in the first entry of
	// the tx buffer.

	uint8_t command = _txBuf[0];

	bool okay = false;

	// Clear the GPIO events before command goes active. This is so the scenario where the Ready For Command signal returned
	// by the Pico rapidly goes high/low can be detected.
	__clearGpioEvents();

	// Single master active/deactive cycle per SPI transfer session. This hopefully should give the transfers a
	// "reset on fault" tolerance because it is assumed the pico resets its SPI transfer when master active goes low.
	__setCommandActive(true);

	// Wait for the Pico to be ready for a command.
	bool activeOkay = __waitForReadyForCommandActive();

	if(activeOkay && __picoSpiTxRx(_txBuf, _rxBuf, PICO_SPI_LATCHED_DATA_CMD_RESP_FRAME_SIZE))
	{
		// Ready for command goes low when a reply is available.
		bool replyReady = __waitForReadyForCommandInactive();

		if(replyReady)
		{
			// Clear the tx buffer so the Pico doesn't get the same command again.
			for(int index = 0; index < TX_RX_BUFFER_SIZE; index++) _txBuf[index] = 0;

			okay = __picoSpiTxRx(_txBuf, _rxBuf, PICO_SPI_LATCHED_DATA_CMD_RESP_FRAME_SIZE);

			// A successful rx transfer can still be the error reply from the Pico.
			if(okay) okay = _rxBuf[0] == command;
		}
	}

	__setCommandActive(false);

	return okay;
}

void LatcherPico::__clearTxBuffer()
{
	for(int index = 0; index < TX_RX_BUFFER_SIZE; index++) _txBuf[index] = 0;
}

int LatcherPico::__downloadLatchedDataIndex(const char* latchedDataIndexName)
{
	int retVal = -1;

	__clearTxBuffer();

	_txBuf[0] = GET_LATCHED_DATA_INDEX;
	_txBuf[1] = latchedDataIndexName[0];
	_txBuf[2] = latchedDataIndexName[1];
	_txBuf[3] = latchedDataIndexName[2];

	bool okay = __sendRecvCommand();

	if(okay)
	{
		retVal = _rxBuf[1];
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

	cout << "Waiting for SKH latched data index.\n";

	// Speed in KMH.
	_picoLatchedDataIndexes[LatcherPico::SPEED_KMH] = __downloadLatchedDataIndex("SKH");

	cout << "SKH Index: "  << _picoLatchedDataIndexes[LatcherPico::SPEED_KMH] << "\n";

	cout << "Waiting for ETC latched data index.\n";

	// Engine temperature degrees celsius.
	_picoLatchedDataIndexes[LatcherPico::ENGINE_TEMP_C] = __downloadLatchedDataIndex("ETC");

	cout << "ETC Index: "  << _picoLatchedDataIndexes[LatcherPico::ENGINE_TEMP_C] << "\n";
}

int LatcherPico::__downloadLatchedDataResolution(int latchedDataIndex)
{
	int retVal = -1;

	__clearTxBuffer();

	_txBuf[0] = GET_LATCHED_DATA_RESOLUTION;
	_txBuf[1] = latchedDataIndex;

	bool okay = __sendRecvCommand();

	if(okay)
	{
		// Data returned is 16 bit integer, little endian byte order.

		retVal = _rxBuf[2];
		retVal <<= 8;
		retVal += _rxBuf[1];
	}

	return retVal;
}

void LatcherPico::__downloadLatchedDataResolutions()
{
	for(int index = 0; index < MAX_LATCHED_INDEXES; index++)
	{
		int latchedDataIndex = _picoLatchedDataIndexes[index];

		if(latchedDataIndex > 0)
		{
			_picoLatchedDataResolutions[index] = __downloadLatchedDataResolution(latchedDataIndex);
		}
	}

	if(_picoLatchedDataIndexes[LatcherPico::ENGINE_RPM] > 0)
	{
		cout << "ERM resolution: " << _picoLatchedDataResolutions[LatcherPico::ENGINE_RPM] << "\n";
	}

	if(_picoLatchedDataIndexes[LatcherPico::SPEED_KMH] > 0)
	{
		cout << "SKH resolution: " << _picoLatchedDataResolutions[LatcherPico::SPEED_KMH] << "\n";
	}

	if(_picoLatchedDataIndexes[LatcherPico::ENGINE_TEMP_C] > 0)
	{
		cout << "ETC resolution: " << _picoLatchedDataResolutions[LatcherPico::ENGINE_TEMP_C] << "\n";
	}
}

void LatcherPico::__downloadLatchedDataRaw()
{
	for(int index = 0; index < MAX_LATCHED_INDEXES; index++)
	{
		int latchedDataIndex = _picoLatchedDataIndexes[index];

		if(latchedDataIndex > 0)
		{
			__clearTxBuffer();

			_txBuf[0] = GET_LATCHED_DATA;
			_txBuf[1] = latchedDataIndex;

			bool okay = __sendRecvCommand();

			if(okay)
			{
				// Data returned is a 32 bit integer, little endian byte order.

				uint32_t coercVal;
				uint32_t rawVal = 0;

				rawVal = _rxBuf[4] << 24;
				coercVal = _rxBuf[3] << 16;
				rawVal += coercVal;
				coercVal = _rxBuf[2] << 8;
				rawVal += coercVal;
				rawVal += _rxBuf[1];

				_picoLatchedDataRaw[index] = rawVal;
			}
		}
	}
}

double LatcherPico::getLatchedDataValueDouble(LatchedDataIndex dataIndex)
{
	double retVal;

	switch (dataIndex)
	{
		case Latcher::ENGINE_RPM:

			retVal = __getLatchedDoubleValue(dataIndex);
			break;

		case Latcher::SPEED_KMH:

			retVal = __getLatchedDoubleValue(dataIndex);
			break;

		case Latcher::ENGINE_TEMP_C:

			retVal = __getLatchedDoubleValue(dataIndex);
			break;

		case Latcher::BOOST_PSI:

			retVal = __getLatchedDoubleValue(dataIndex);
			break;

		default:

			retVal = 0;
			break;
	}

	return retVal;
}

bool LatcherPico::getLatchedDataValueBool(LatchedDataIndex dataIndex)
{
	// TODO ... Indicators etc.
	return false;
}

double LatcherPico::__getLatchedDoubleValue(int latchedDataIndex)
{
	if(_picoLatchedDataIndexes[latchedDataIndex] > 0 && _picoLatchedDataResolutions[latchedDataIndex] != 0)
	{
		return (double)_picoLatchedDataRaw[latchedDataIndex] / (double)_picoLatchedDataResolutions[latchedDataIndex];
	}

	return 0;
}

bool LatcherPico::__getLatchedBoolValue(int latchedDataIndex)
{
	if(_picoLatchedDataIndexes[latchedDataIndex] > 0)
	{
		return _picoLatchedDataRaw[latchedDataIndex] != 0;
	}

	return false;
}
