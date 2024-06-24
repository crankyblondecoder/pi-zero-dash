#include <iostream>
#include "InstrumentOnOff.hpp"

using namespace piZeroDash;

InstrumentOnOff::~InstrumentOnOff()
{
}

InstrumentOnOff::InstrumentOnOff()
{
}

bool InstrumentOnOff::latch()
{
	bool latched = _unlatched;

	if(_unlatched) _unlatched = false;

	if(_getInTestMode())
	{
		double testVal = _getNumericalTestValue();

		bool nextLatchedValue = (unsigned)testVal % 2;

		if(_latchedValue != nextLatchedValue)
		{
			_latchedValue = nextLatchedValue;
			latched = true;
		}
	}
	else
	{
		// TODO ... Get actual data from pico based on some kind of switch selector?
	}

	return latched;
}

bool InstrumentOnOff::getOnOffState()
{
	return _latchedValue;
}

void InstrumentOnOff::test()
{
	// This should latch once per second.
	_testNumerical(0.0, 16.0, 2.0 / 1000.0, false);
}