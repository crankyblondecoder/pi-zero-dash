#include <cmath>
#include <iostream>

using namespace std;

#include "InstrumentBoost.hpp"
#include "Latcher.hpp"

using namespace piZeroDash;

InstrumentBoost::~InstrumentBoost()
{
}

InstrumentBoost::InstrumentBoost()
{
}

bool InstrumentBoost::latch()
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
			double latchedVal = curLatcher -> getLatchedDataValueDouble(Latcher::BOOST_PSI);

			if(_latchedValue != latchedVal)
			{
				_latchedValue = latchedVal;
				newVal = true;
			}
		}
	}

	return newVal;
}

double InstrumentBoost::getBoost()
{
	return _latchedValue;
}

void InstrumentBoost::test(double minBoost, double maxBoost)
{
	_testNumerical(minBoost, maxBoost, (maxBoost - minBoost) / 6000.0, false);
}
