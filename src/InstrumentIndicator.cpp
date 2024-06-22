#include <iostream>
#include "InstrumentIndicator.hpp"

using namespace piZeroDash;

InstrumentIndicator::~InstrumentIndicator()
{
}

InstrumentIndicator::InstrumentIndicator()
{
}

bool InstrumentIndicator::latch()
{
	if(_getInTestMode())
	{
		double testVal = _getNumericalTestValue();

		InstrumentIndicator::IndicatorState nextLatchedValue;

		if(testVal - (double)((int)testVal) > 0.5)
		{
			// Should be off half the time.
			nextLatchedValue = InstrumentIndicator::NONE;
		}
		else
		{
			if(testVal < 5.0)
			{
				nextLatchedValue = InstrumentIndicator::LEFT;
			}
			else if(testVal < 10.0)
			{
				nextLatchedValue = InstrumentIndicator::RIGHT;
			}
			else
			{
				nextLatchedValue = InstrumentIndicator::BOTH;
			}
		}

		if(_latchedValue != nextLatchedValue)
		{
			_latchedValue = nextLatchedValue;
			return true;
		}

		return false;
	}

	// TODO ... Get actual indicator data from pico.
	return false;
}

InstrumentIndicator::IndicatorState InstrumentIndicator::getIndicatorState()
{
	return _latchedValue;
}

void InstrumentIndicator::test()
{
	_testNumerical(0.0, 15.0, 1.0 / 1000.0, true);
}