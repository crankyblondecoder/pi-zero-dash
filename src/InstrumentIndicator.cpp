#include <iostream>
#include "InstrumentIndicator.hpp"

using namespace piZeroDash;

InstrumentIndicator::~InstrumentIndicator()
{
}

InstrumentIndicator::InstrumentIndicator() : _inTestMode{false}
{
}

bool InstrumentIndicator::latch()
{
	if(_inTestMode)
	{
		if(_curTestCycleNumber > _curTestNumberCycles)
		{
			struct timeval curTime;

			gettimeofday(&curTime, 0);

			long millisSinceLastLatch = (curTime.tv_sec - _testLastSec) * 1000 + (curTime.tv_usec - _testLastUSec) / 1000;

			if(millisSinceLastLatch > 2000)
			{
				_testLastSec = curTime.tv_sec;
				_testLastUSec = curTime.tv_usec;

				bool testSingleStep = millisSinceLastLatch > 500;
			}
		}
		else
		{
			_inTestMode = false;
		}

		return _inTestMode;
	}

	// TODO ... Get actual indicator data from pico.
	return false;
}

InstrumentIndicator::IndicatorState InstrumentIndicator::getIndicatorState()
{
	return _latchedValue;
}

void InstrumentIndicator::test(unsigned numberCycles)
{
	_curTestNumberCycles = numberCycles;
	_curTestCycleNumber = 0;
	_curTestValue = InstrumentIndicator::IndicatorState::NONE;
	_testSingleStep = false;

	_inTestMode = true;
}

bool InstrumentIndicator::inTestMode()
{
	return _inTestMode;
}