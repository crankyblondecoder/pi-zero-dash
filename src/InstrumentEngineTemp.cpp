#include <iostream>

using namespace std;

#include "InstrumentEngineTemp.hpp"

using namespace piZeroDash;

InstrumentEngineTemp::~InstrumentEngineTemp()
{
}

InstrumentEngineTemp::InstrumentEngineTemp()
{
}

bool InstrumentEngineTemp::latch()
{
	if(inTestMode())
	{
		int testVal = _getNumericalTestValue();

		if(_latchedValue != testVal)
		{
			_latchedValue = testVal;
			return true;
		}

		return false;
	}

	// TODO ... Get actual rpm data from pico.
	return false;
}

int InstrumentEngineTemp::getEngineTemp()
{
	return _latchedValue;
}

void InstrumentEngineTemp::test(int minTemp, int maxTemp)
{
	double range = maxTemp - minTemp;
	double secondsForRange = 6.0;

	_testNumerical(minTemp, maxTemp, range / secondsForRange / 1000.0, false);
}
