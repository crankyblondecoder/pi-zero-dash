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
	if(_getInTestMode())
	{
		double testVal = _getNumericalTestValue();

		bool nextLatchedValue = (unsigned)testVal % 2;

		if(_latchedValue != nextLatchedValue)
		{
			_latchedValue = nextLatchedValue;
			return true;
		}

		return false;
	}

	// TODO ... Get actual data from pico based on some kind of switch selector?
	return false;
}

bool InstrumentOnOff::getOnOffState()
{
	return _latchedValue;
}

void InstrumentOnOff::test()
{
	// This should latch once per second.
	_testNumerical(0.0, 6.0, 1.0 / 1000.0, true);
}