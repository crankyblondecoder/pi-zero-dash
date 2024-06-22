#include "Instrument.hpp"

using namespace piZeroDash;

Instrument::~Instrument()
{
}

Instrument::Instrument()
{
}

void Instrument::_testNumerical(double min, double max, double testStepPerMillis, bool forwardOnly)
{
	_numTestMin = min;
	_numTestMax = max;
	_numTestStep = testStepPerMillis;
	_numTestForwardOnly = forwardOnly;

	_curNumTestValue = min;
	_numTestForwardDirection = true;

	_testSingleStep = false;

	_inTestMode = true;
}

bool Instrument::inTestMode()
{
	return _inTestMode;
}

bool Instrument::_getInTestMode()
{
	return _inTestMode;
}

double Instrument::_getNumericalTestValue()
{
	struct timeval curTime;

	gettimeofday(&curTime, 0);

	double millisSinceLastLatch = (curTime.tv_sec - _testLastSec) * 1000 + (curTime.tv_usec - _testLastUSec) / 1000;

	_testLastSec = curTime.tv_sec;
	_testLastUSec = curTime.tv_usec;

	bool testSingleStep = millisSinceLastLatch > 500;

	double millis = (curTime.tv_sec - _testStartSec) * 1000 + (curTime.tv_usec - _testStartUSec) / 1000;

	if(_numTestForwardDirection)
	{
		if(_curNumTestValue == _numTestMin)
		{
			_testStartSec = curTime.tv_sec;
			_testStartUSec = curTime.tv_usec;

			// Bump the value so that the current test value increases on the next call to this function.
			_curNumTestValue += _numTestStep;
		}
		else
		{
			if(testSingleStep)
			{
				_curNumTestValue += _numTestStep;
			}
			else
			{
				_curNumTestValue = _numTestMin + millis * _numTestStep;
			}
		}

		if(_curNumTestValue >= _numTestMax)
		{
			_numTestForwardDirection = false;

			_testStartSec = curTime.tv_sec;
			_testStartUSec = curTime.tv_usec;
		}
	}
	else if(!_numTestForwardOnly && _curNumTestValue > _numTestMin)
	{
		if(testSingleStep)
		{
			_curNumTestValue -= _numTestStep;
		}
		else
		{
			_curNumTestValue = _numTestMax - millis * _numTestStep;
		}
	}
	else
	{
		_inTestMode = false;
	}

	return _curNumTestValue;
}