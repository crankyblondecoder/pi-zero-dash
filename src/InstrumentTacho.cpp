#include <iostream>
#include "InstrumentTacho.hpp"

using namespace piZeroDash;

InstrumentTacho::~InstrumentTacho()
{
}

InstrumentTacho::InstrumentTacho() : _inTestMode{false}
{
	_curTestValue = 0;
}

bool InstrumentTacho::latch()
{
	if(_inTestMode)
	{
		struct timeval curTime;

		gettimeofday(&curTime, 0);

		long millisSinceLastLatch = (curTime.tv_sec - _testLastSec) * 1000 + (curTime.tv_usec - _testLastUSec)/1000;

		_testLastSec = curTime.tv_sec;
		_testLastUSec = curTime.tv_usec;

		bool testSingleStep = millisSinceLastLatch > 500;

		long millis = (curTime.tv_sec - _testStartSec) * 1000 + (curTime.tv_usec - _testStartUSec)/1000;

		if(testSingleStep)
		{
			std::cout << "single step\n";
		}
		else
		{
			std::cout << "millis: " << millis << "\n";
		}

		if(_curTestForwardDirection)
		{
			if(_curTestValue == 0.0)
			{
				_curTestValue = 500;

				_testStartSec = curTime.tv_sec;
				_testStartUSec = curTime.tv_usec;
			}
			else
			{
				if(testSingleStep)
				{
					_curTestValue += 50;
				}
				else
				{
					_curTestValue = 500 + millis;
				}
			}

			if(_curTestValue >= _curTestMaxRpm)
			{
				_curTestForwardDirection = false;

				_testStartSec = curTime.tv_sec;
				_testStartUSec = curTime.tv_usec;
			}
		}
		else if(_curTestValue > 0)
		{
			if(testSingleStep)
			{
				_curTestValue -= 50;
			}
			else
			{
				_curTestValue = _curTestMaxRpm - millis;
			}
		}
		else
		{
			_inTestMode = false;
		}

		if(_curTestValue >= 0)
		{
			_latchedValue = _curTestValue;
		}
		else
		{
			_latchedValue = 0;
		}

		return _inTestMode;
	}

	// TODO ... Get actual rpm data from pico.
	return false;
}

unsigned InstrumentTacho::getRpm()
{
	return _latchedValue;
}

void InstrumentTacho::test(unsigned maxRpm)
{
	_curTestMaxRpm = maxRpm;
	_curTestValue = 0;
	_curTestForwardDirection = true;
	_testSingleStep = false;

	_inTestMode = true;
}

bool InstrumentTacho::inTestMode()
{
	return _inTestMode;
}