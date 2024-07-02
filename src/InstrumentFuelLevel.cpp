#include <iostream>

using namespace std;

#include "InstrumentFuelLevel.hpp"

using namespace piZeroDash;

InstrumentFuelLevel::~InstrumentFuelLevel()
{
}

InstrumentFuelLevel::InstrumentFuelLevel()
{
}

bool InstrumentFuelLevel::latch()
{
	if(inTestMode())
	{
		double testVal = _getNumericalTestValue();

		if(_latchedValue != testVal)
		{
			_latchedValue = testVal;
			return true;
		}

		return false;
	}

	// TODO ... Get actual fuel level data from pico.
	return false;
}

unsigned InstrumentFuelLevel::getFuelLitres()
{
	return _latchedValue;
}

void InstrumentFuelLevel::test(unsigned maxFuelLitres)
{
	_testNumerical(0.0, maxFuelLitres, (double) maxFuelLitres / 8000.0, false);
}
