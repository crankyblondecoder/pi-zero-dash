#include <cmath>
#include <iostream>

using namespace std;

#include "InstrumentBoost.hpp"

using namespace piZeroDash;

InstrumentBoost::~InstrumentBoost()
{
}

InstrumentBoost::InstrumentBoost()
{
}

bool InstrumentBoost::latch()
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

	// TODO ... Get actual boost data from pico.
	return false;
}

double InstrumentBoost::getBoost()
{
	return _latchedValue;
}

void InstrumentBoost::test(double minBoost, double maxBoost)
{
	_testNumerical(minBoost, maxBoost, (maxBoost - minBoost) / 6000.0, false);
}
