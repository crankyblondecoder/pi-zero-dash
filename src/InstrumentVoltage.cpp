#include <cmath>
#include <iostream>

using namespace std;

#include "InstrumentVoltage.hpp"

using namespace piZeroDash;

InstrumentVoltage::~InstrumentVoltage()
{
}

InstrumentVoltage::InstrumentVoltage()
{
}

bool InstrumentVoltage::latch()
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

double InstrumentVoltage::getVoltage()
{
	return _latchedValue;
}

void InstrumentVoltage::test(double maxVoltage)
{
	_testNumerical(0.0, maxVoltage, maxVoltage / 6000.0, false);
}
