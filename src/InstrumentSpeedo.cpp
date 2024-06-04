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

		long millis = (curTime.tv_sec - _testStartSec) * 1000 + (curTime.tv_usec - _testStartUSec)/1000;

		std::cout << "millis: " << millis << "\n";

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
				_curTestValue = 10 + (millis / 50);
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
			_curTestValue = _curTestMaxSpeed - (millis / 75);
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

	_inTestMode = true;
}

bool InstrumentSpeedo::inTestMode()
{
	return _inTestMode;
}