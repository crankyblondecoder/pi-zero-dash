#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeFuelLevel_e36.hpp"

using namespace piZeroDash;

GaugeFuelLevel_e36::~GaugeFuelLevel_e36()
{
}

GaugeFuelLevel_e36::GaugeFuelLevel_e36(int maxFuelLevel, int globalPositionX, int globalPositionY, unsigned width,
	unsigned height)
	: GaugeFuelLevel(maxFuelLevel, (double) width / 2.0, (double) width / 2.0, (double) width / 2.0, globalPositionX,
	globalPositionY, width, height)
{
	double markedFontSize = 0.07 * (double) width;
	_lineLength = 0.1 * (double) width;
	_majorLineWidth = 0.01 * (double) width;
	double minorLineWidth = 0.005 * (double) width;
	double lineStartOffset = _lineLength * 0.1;

	// Just default to use indicator line for now.
	_setStandardProperties(markedFontSize, _markedFontColour, 0, _lineLength, _majorLineWidth, minorLineWidth, lineStartOffset,
		_majorLineColour, _minorLineColour, M_PI * (165.0 / 180.0), M_PI * (285.0 / 180.0),
		(double) maxFuelLevel * (12.5 / 100.0), _lowFuelLevelColour, _belowZeroFuelLevelColour);
}

void GaugeFuelLevel_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface);
}

void GaugeFuelLevel_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _majorLineWidth * 3.0, _indiactorLineColour);
}
