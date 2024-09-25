#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <unistd.h>

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

	// TODO ... Console output to indicate error
	blah;

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
					}
				}
			}
		}
	}
}

void LatcherPico::_poll()
{
	if(_gpioChipFd > -1 && _gpioLineFd > -1)
	{
		// TODO ... Take the "master active" GPIO high to indicate to the pico that comms is active.

		// TODO ... Take the "master active" GPIO low to indicate to the pico that comms is deactive.
	}
}

bool LatcherPico::_isReady()
{
	return _ready;
}

void LatcherPico::__picoSpiTxRx(uint8_t* rxBuf, uint8_t* txBuf, int length)
{
	if(_spiFd > -1)
	{
	}
}
