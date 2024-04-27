#include "GaugeSpeedo.hpp"

using namespace piZeroDash;

GaugeSpeedo::~GaugeSpeedo()
{
}

GaugeSpeedo::GaugeSpeedo(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Gauge(globalPositionX, globalPositionY, width, height)
{
	_maxSpeed = maxSpeed;
}

unsigned GaugeSpeedo::_getMaxSpeed()
{
	return _maxSpeed;
}

void GaugeSpeedo::test()
{
	_speedoInstr.test(_maxSpeed);
}
