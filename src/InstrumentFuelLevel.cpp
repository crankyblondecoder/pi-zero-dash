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

double InstrumentFuelLevel::getFuelVolume()
{
	return _latchedValue;
}

void InstrumentFuelLevel::test(unsigned maxFuelLitres)
{
	_testNumerical(-2.0, maxFuelLitres, (double) maxFuelLitres / 6000.0, false);
}
