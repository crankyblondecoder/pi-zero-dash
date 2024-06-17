#include <iostream>
#include "InstrumentSpeedo.hpp"

using namespace piZeroDash;

InstrumentSpeedo::~InstrumentSpeedo()
{
}

InstrumentSpeedo::InstrumentSpeedo()
{
}

bool InstrumentSpeedo::latch()
{
	if(_getInTestMode())
	{
		_latchedValue = _getNumericalTestValue();
		return true;
	}

	// TODO ... Get actual speedo data from pico.
	return false;
}

unsigned InstrumentSpeedo::getSpeed()
{
	return _latchedValue;
}

void InstrumentSpeedo::test(unsigned maxSpeed)
{
	_testNumerical(0.0, maxSpeed, 1.0/30.0);
}