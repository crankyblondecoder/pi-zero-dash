#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeBoost_e36.hpp"

using namespace piZeroDash;

GaugeBoost_e36::~GaugeBoost_e36()
{
}

GaugeBoost_e36::GaugeBoost_e36(int minBoost, int maxBoost, int neutralBoost, int globalPositionX,
	int globalPositionY, unsigned width, unsigned height)
	: GaugeBoost(minBoost, maxBoost, neutralBoost, (double) width / 2.0, (double) width / 2.0,
	 (double) width / 2.0, globalPositionX, globalPositionY, width, height)
{
	double markedFontSize = 0.07 * (double) width;
	_lineLength = 0.1 * (double) width;
	double majorLineWidth = 0.01 * (double) width;
	double minorLineWidth = 0.005 * (double) width;
	double lineStartOffset = _lineLength * 0.1;

	_setStandardProperties(markedFontSize, _markedFontColour, 0, _lineLength, majorLineWidth, minorLineWidth, lineStartOffset,
		_majorLineColour, _minorLineColour, _belowNeutralIndictatorColour, _aboveNeutralIndictatorColour, 0,
		-M_PI / 2.0);
}

void GaugeBoost_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface);
}

void GaugeBoost_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2);
}
