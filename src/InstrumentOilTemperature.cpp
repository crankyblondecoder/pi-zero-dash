#include <cmath>
#include <iostream>

using namespace std;

#include "InstrumentOilTemperature.hpp"

using namespace piZeroDash;

InstrumentOilTemperature::~InstrumentOilTemperature()
{
}

InstrumentOilTemperature::InstrumentOilTemperature()
{
}

bool InstrumentOilTemperature::latch()
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

	// TODO ... Get actual data from pico.
	return false;
}

double InstrumentOilTemperature::getOilTemperature()
{
	return _latchedValue;
}

void InstrumentOilTemperature::test(double maxOilTemperature)
{
	_testNumerical(-10.0, maxOilTemperature, maxOilTemperature / 6000.0, false);
}
