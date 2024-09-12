#include <fcntl.h>
#include <linux/spi/spidev.h>
#include <unistd.h>

#include "LatcherPico.hpp"

using namespace piZeroDash;

LatcherPico::~LatcherPico()
{
	if(_spiFd > -1) close(_spiFd);
}

LatcherPico::LatcherPico()
{
	// TODO ... Setup SPI comms to the pico.

	_spiFd = open("/dev/spidev0.0", O_RDWR);

	if(_spiFd > -1)
	{
		// TODO ...
		blah;
	}
}

void LatcherPico::_poll()
{

}