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
	: GaugeSpeedo(maxSpeed, globalPositionX, globalPositionY, width, height)
{
	// GaugeSpeedo_e36(unsigned maxSpeed, double markedSpeedFontSize, double lineLength, double majorLineWidth, double minorLineWidth,
	//	int globalPositionX, int globalPositionY, unsigned width, unsigned height);

	//_speedo = new GaugeSpeedo_e36(260, 0.05 * (double) speedoWidth, 0.05 * (double) speedoWidth, 0.01 * (double) speedoWidth,
	//	0.005 * (double) speedoWidth, ((dashWidth - speedoWidth) / 2), dashHeight - speedoHeight, speedoWidth, speedoHeight);


	_markedSpeedFontSize = 0.05 * (double) width;
	_lineLength = 0.05 * (double) width;
	_majorLineWidth = 0.01 * (double) width;
	_minorLineWidth = 0.005 * (double) width;
	_lineStartOffset = _lineLength * 0.1;

	_preciseSpeedBackgroundWidth = 0.2 * (double) width;
	_preciseSpeedBackgroundHeight = 0.1 * (double) width;
}

void GaugeSpeedo_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface, _markedSpeedFontSize, _markedSpeedFontColour, _lineLength, _majorLineWidth, _minorLineWidth,
		_lineStartOffset, _majorLineColour, _minorLineColour, _preciseSpeedBackgroundColour, _preciseSpeedBackgroundWidth,
		_preciseSpeedBackgroundHeight);

	//_drawDefaultBackground(surface, _markedSpeedFontSize, _lineLength, _majorLineWidth, _minorLineWidth,
	//	_lineLength * 0.1);
}

void GaugeSpeedo_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _majorLineWidth * 2);
}
