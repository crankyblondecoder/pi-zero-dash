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
		double testVal = _getNumericalTestValue();

		if(_latchedValue != testVal)
		{
			_latchedValue = testVal;
			return true;
		}

		return false;
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
	_testNumerical(0.0, maxSpeed, 1.0/30.0, false);
}