#include <cmath>
#include <iostream>

using namespace std;

#include "InstrumentOilPressure.hpp"

using namespace piZeroDash;

InstrumentOilPressure::~InstrumentOilPressure()
{
}

InstrumentOilPressure::InstrumentOilPressure()
{
}

bool InstrumentOilPressure::latch()
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

double InstrumentOilPressure::getOilPressure()
{
	return _latchedValue;
}

void InstrumentOilPressure::test(double maxOilPressure)
{
	_testNumerical(0.0, maxOilPressure, maxOilPressure / 6000.0, false);
}
