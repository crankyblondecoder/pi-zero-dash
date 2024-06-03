#include "Gauge.hpp"

using namespace piZeroDash;

Gauge::~Gauge()
{
	delete[] _instruments;
}

Gauge::Gauge(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Visual(globalPositionX, globalPositionY, width, height, true, true)

{
	_instruments = new Instrument*[NUM_INSTRUMENTS];
	_instrumentCount = 0;
}

void Gauge::scan()
{
	// Latch instruments and if any have changed draw the foreground;
	bool draw = false;

	for(unsigned index = 0; index < _instrumentCount; index++)
	{
		draw = draw || _instruments[index] -> latch();
	}

	if(draw)
	{
		_drawForeground();
		_composeForegroundToDisplay();
	}
}

void Gauge::_addInstrument(Instrument* instrument)
{
	_instruments[_instrumentCount++] = instrument;
}
