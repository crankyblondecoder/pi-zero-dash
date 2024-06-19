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

		if(testVal - (double)((int)testVal) > 0.5)
		{
			// Should be off half the time.
			_latchedValue = InstrumentIndicator::NONE;
		}
		else
		{
			if(testVal < 5.0)
			{
				_latchedValue = InstrumentIndicator::LEFT;
			}
			else if(testVal < 10.0)
			{
				_latchedValue = InstrumentIndicator::RIGHT;
			}
			else
			{
				_latchedValue = InstrumentIndicator::BOTH;
			}
		}

		return true;
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
	_testNumerical(0.0, 15.0, 1.0 / 1000.0);
}