#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "LatcherPico.hpp"

using namespace piZeroDash;

LatcherPico::~LatcherPico()
{
	if(_spiFd > -1) close(_spiFd);
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
				// Assume ready has been initialised to false prior.
				_ready = true;
			}
		}
	}
}

void LatcherPico::_poll()
{

}

bool LatcherPico::_isReady()
{
	return _ready;
}

void LatcherPico::__picoSpiTxRx(uint8_t* rxBuf, uint8_t* txBuf, int length)
{


}
