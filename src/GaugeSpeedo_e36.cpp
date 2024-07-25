#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeSpeedo_e36.hpp"

using namespace piZeroDash;

GaugeSpeedo_e36::~GaugeSpeedo_e36()
{
}

GaugeSpeedo_e36::GaugeSpeedo_e36(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeSpeedo((double) width / 2.0, (double) width / 2.0, (double) width / 2.0, maxSpeed, globalPositionX, globalPositionY,
	width, height)
{
	_markedSpeedFontSize = 0.05 * (double) width;
	_lineLength = 0.05 * (double) width;
	_majorLineWidth = 0.01 * (double) width;
	_minorLineWidth = 0.005 * (double) width;
	_lineStartOffset = _lineLength * 0.1;

	_preciseSpeedBackgroundWidth = 0.2 * (double) width;
	_preciseSpeedBackgroundHeight = 0.1 * (double) width;

	_setProperties(_markedSpeedFontSize, _markedSpeedFontColour, 0, _lineLength, _majorLineWidth, _minorLineWidth,
		_lineStartOffset, _majorLineColour, _minorLineColour, _preciseSpeedBackgroundColour, _preciseSpeedBackgroundWidth,
		_preciseSpeedBackgroundHeight, "km/h");
}

void GaugeSpeedo_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface);
}

void GaugeSpeedo_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _majorLineWidth * 2, _indiactorLineColour,
		_preciseSpeedBackgroundHeight / 2.0, _preciseSpeedFontColour);
}

bounds GaugeSpeedo_e36::getPreciseSpeedBoxBounds()
{
	struct bounds precSpeedBox = _calcPreciseSpeedBoxBounds(_preciseSpeedBackgroundWidth, _preciseSpeedBackgroundHeight);

	double globalPosnX = _getGlobalPositionX();
	double globalPosnY = _getGlobalPositionY();

	precSpeedBox.left += globalPosnX;
	precSpeedBox.right += globalPosnX;
	precSpeedBox.top += globalPosnY;
	precSpeedBox.bottom += globalPosnY;

	return precSpeedBox;
}
