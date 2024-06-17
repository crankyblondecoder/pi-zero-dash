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
	if(inTestMode())
	{
		_latchedValue = _getNumericalTestValue();
		return true;
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
	_testNumerical(0.0, maxRpm, 1.0);
}