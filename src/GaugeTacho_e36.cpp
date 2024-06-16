#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeTacho_e36.hpp"

using namespace piZeroDash;

GaugeTacho_e36::~GaugeTacho_e36()
{
}

GaugeTacho_e36::GaugeTacho_e36(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, bool flashRedline,
	int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeTacho(maxRpm, redlineRpm, redlineWarningRpm, flashRedline, globalPositionX, globalPositionY, width, height)
{
	_markedRpmFontSize = 0.07 * (double) width;
	_lineLength = 0.1 * (double) width;
	_majorLineWidth = 0.01 * (double) width;
	_minorLineWidth = 0.005 * (double) width;
	_lineStartOffset = _lineLength * 0.1;
}

void GaugeTacho_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface, _markedRpmFontSize, _markedRpmFontColour, 0, _lineLength, _majorLineWidth,
		_minorLineWidth, _lineStartOffset, _majorLineColour, _minorLineColour);
}

void GaugeTacho_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _normalRpmIndictatorColour, _redlineWarningRpmIndictatorColour,
		_redlineRpmIndictatorColour);
}
