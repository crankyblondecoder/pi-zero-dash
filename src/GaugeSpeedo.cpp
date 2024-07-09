#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeSpeedo.hpp"

using namespace piZeroDash;

GaugeSpeedo::~GaugeSpeedo()
{
}

GaugeSpeedo::GaugeSpeedo(double radius, double dialCentreX, double dialCentreY, unsigned maxSpeed, int globalPositionX,
	int globalPositionY, unsigned width, unsigned height)
	: GaugeDial(radius, dialCentreX, dialCentreY, globalPositionX, globalPositionY, width, height)
{
	_maxSpeed = maxSpeed;

	_addInstrument(&_speedoInstr);
}

unsigned GaugeSpeedo::_getMaxSpeed()
{
	return _maxSpeed;
}

void GaugeSpeedo::test()
{
	_speedoInstr.test(_maxSpeed + 10);
}

bool GaugeSpeedo::inTestMode()
{
	return _speedoInstr.inTestMode();
}

bounds GaugeSpeedo::_calcPreciseSpeedBoxBounds(double width, double height)
{
	return _calcPreciseValueBoxBounds(width, height);
}

void GaugeSpeedo::_setProperties(double markedSpeedFontSize, colour& markedSpeedFontColour,
	unsigned markedSpeedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
	double lineStartOffset, colour& majorLineColour, colour& minorLineColour, colour& preciseSpeedBackgroundColour,
	double preciseSpeedBackgroundWidth, double preciseSpeedBackgroundHeight)
{
	_setStandardProperties(20, _getMaxSpeed(), 10, true, true, true, markedSpeedFontSize,
		markedSpeedFontColour, markedSpeedFontDecimalPlaces, lineLength, majorLineWidth, minorLineWidth, lineStartOffset,
		majorLineColour, minorLineColour, M_PI, 2.0 * M_PI);

	_preciseSpeedBackgroundColour = preciseSpeedBackgroundColour;
	_preciseSpeedBackgroundWidth = preciseSpeedBackgroundWidth;
	_preciseSpeedBackgroundHeight = preciseSpeedBackgroundHeight;
}

void GaugeSpeedo::_drawDefaultBackground(CairoSurface& surface)
{
	GaugeDial::_drawDefaultBackground(surface);

	_drawStandardPreciseBoxBackground(surface, _preciseSpeedBackgroundWidth, _preciseSpeedBackgroundHeight,
		_preciseSpeedBackgroundColour);
}

void GaugeSpeedo::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
	colour& indicatorLineColour, double preciseSpeedFontSize, colour& preciseSpeedFontColour)
{
	unsigned curSpeed = _speedoInstr.getSpeed();

	_drawStandardIndicatorLine(surface, (double) curSpeed, indicatorLineLength, indicatorLineWidth, indicatorLineColour);

	_drawStandardPreciseBoxForeground(surface, curSpeed, 0, "", preciseSpeedFontSize, preciseSpeedFontColour);
}
