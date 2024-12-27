#include <iostream>

using namespace std;

#include "InstrumentTacho.hpp"

using namespace piZeroDash;

InstrumentTacho::~InstrumentTacho()
{
}

InstrumentTacho::InstrumentTacho()
{
}

bool InstrumentTacho::latch()
{
	bool newVal = false;

	if(inTestMode())
	{
		double testVal = _getNumericalTestValue();

		if(_latchedValue != testVal)
		{
			_latchedValue = testVal;
			newVal = true;
		}
	}
	else
	{
		Latcher* curLatcher = _getLatcher();

		if(curLatcher)
		{
			double latchedVal = curLatcher -> getLatchedDataValueDouble(Latcher::ENGINE_RPM);

			if(_latchedValue != latchedVal)
			{
				_latchedValue = latchedVal;
				newVal = true;
			}
		}
	}

	return newVal;
}

unsigned InstrumentTacho::getRpm()
{
	return _latchedValue;
}

void InstrumentTacho::test(unsigned maxRpm)
{
	_testNumerical(0.0, maxRpm, 1.0, false);
}
