#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeSpeedo_e36.hpp"

using namespace piZeroDash;

GaugeSpeedo_e36::~GaugeSpeedo_e36()
{
}

GaugeSpeedo_e36::GaugeSpeedo_e36(unsigned maxSpeed, double markedSpeedFontSize, double lineLength, double majorLineWidth,
	double minorLineWidth, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeSpeedo(maxSpeed, globalPositionX, globalPositionY, width, height)
{
	_markedSpeedFontSize = markedSpeedFontSize;
	_lineLength = lineLength;
	_majorLineWidth = majorLineWidth;
	_minorLineWidth = minorLineWidth;
}

void GaugeSpeedo_e36::_drawBackground(CairoSurface& surface)
{
	// Used default gauge.
	_drawDefaultBackground(surface, _markedSpeedFontSize, _lineLength, _majorLineWidth, _minorLineWidth);
}

void GaugeSpeedo_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _majorLineWidth * 2);
}
