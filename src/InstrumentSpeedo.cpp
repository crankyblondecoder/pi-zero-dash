#include <iostream>
#include "InstrumentSpeedo.hpp"

using namespace piZeroDash;

InstrumentSpeedo::~InstrumentSpeedo()
{
}

InstrumentSpeedo::InstrumentSpeedo() : _inTestMode{false}
{
	_curTestValue = 0;
}

bool InstrumentSpeedo::latch()
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

		/*
		if(testSingleStep)
		{
			std::cout << "single step\n";
		}
		else
		{
			std::cout << "millis: " << millis << "\n";
		}
		*/

		if(_curTestForwardDirection)
		{
			if(_curTestValue == 0.0)
			{
				_curTestValue = 10;

				_testStartSec = curTime.tv_sec;
				_testStartUSec = curTime.tv_usec;
			}
			else
			{
				if(testSingleStep)
				{
					_curTestValue++;
				}
				else
				{
					_curTestValue = 10 + (millis / 30);
				}
			}

			if(_curTestValue >= _curTestMaxSpeed)
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
				_curTestValue--;
			}
			else
			{
				_curTestValue = _curTestMaxSpeed - (millis / 30);
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

	// TODO ... Get actual speedo data from pico.
	return false;
}

unsigned InstrumentSpeedo::getSpeed()
{
	return _latchedValue;
}

void InstrumentSpeedo::test(unsigned maxSpeed)
{
	_curTestMaxSpeed = maxSpeed;
	_curTestValue = 0;
	_curTestForwardDirection = true;
	_testSingleStep = false;

	_inTestMode = true;
}

bool InstrumentSpeedo::inTestMode()
{
	return _inTestMode;
}