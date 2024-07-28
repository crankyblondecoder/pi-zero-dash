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
	_majorLineWidth = 0.01 * (double) width;
	double minorLineWidth = 0.005 * (double) width;
	double lineStartOffset = _lineLength * 0.1;

	// Just default to use indicator line for now.
	_setStandardProperties(markedFontSize, _markedFontColour, 0, _lineLength, _majorLineWidth, minorLineWidth, lineStartOffset,
		_majorLineColour, _minorLineColour, _belowNeutralIndictatorColour, _aboveNeutralIndictatorColour, false, 0, -M_PI / 2.0);
}

void GaugeBoost_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface);

	double width = _getWidth();

	double symbolWidth = width * 0.12;
	double symbolHeight = symbolWidth * 0.8;

	double dialCentreX = _getDialCentreX();
	double dialCentreY = _getDialCentreY();

	double dialCentreOffset = width * 0.05;

	cairo_t* cr = surface.getContext();

	cairo_identity_matrix(cr);

	_drawTurboPath(cr, dialCentreX + dialCentreOffset, dialCentreX + symbolWidth + dialCentreOffset,
		dialCentreY - symbolHeight - dialCentreOffset, dialCentreY - dialCentreOffset);

	cairo_set_source_rgba(cr, _majorLineColour.r, _majorLineColour.g, _majorLineColour.b, _majorLineColour.a);
	cairo_set_line_width(cr, symbolHeight * 0.07);

	cairo_stroke(cr);
}

void GaugeBoost_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _lineLength * 1.2, _lineLength * 1.2, _majorLineWidth * 3.0, _indiactorLineColour);
}
