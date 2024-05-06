#include "InstrumentSpeedo.hpp"

using namespace piZeroDash;

InstrumentSpeedo::~InstrumentSpeedo()
{
}

InstrumentSpeedo::InstrumentSpeedo() : _inTestMode{false}
{
}

bool InstrumentSpeedo::latch()
{
	if(_inTestMode)
	{
		if(_curTestForwardDirection)
		{
			_latchedValue = _curTestValue++;

			if(_curTestValue >= _curTestMaxSpeed) _curTestForwardDirection = false;
		}
		else if(_curTestValue > 0)
		{
			_latchedValue = _curTestValue--;
		}
		else
		{
			_inTestMode = false;
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
}

bool InstrumentSpeedo::inTestMode()
{
	return _inTestMode;
}