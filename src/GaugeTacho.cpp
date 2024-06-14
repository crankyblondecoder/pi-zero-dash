#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeTacho.hpp"

using namespace piZeroDash;

GaugeTacho::~GaugeTacho()
{
}

GaugeTacho::GaugeTacho(unsigned maxRpm, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeDial(globalPositionX, globalPositionY, width, height)
{
	_maxRpm = maxRpm;

	_addInstrument(&_tachoInstr);
}

unsigned GaugeTacho::_getMaxRpm()
{
	return _maxRpm;
}

void GaugeTacho::test()
{
	_tachoInstr.test(_maxRpm + 500);
}

bool GaugeTacho::inTestMode()
{
	return _tachoInstr.inTestMode();
}

void GaugeTacho::_drawDefaultBackground(CairoSurface& surface, double markedRpmFontSize, colour& markedRpmFontColour,
	unsigned markedRpmFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
	double lineStartOffset, colour& majorLineColour, colour& minorLineColour)
{
	_drawDefaultDialIndicatorBackground(surface, 0, _getMaxRpm() / 1000, 1, true, true, true, markedRpmFontSize,
		markedRpmFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour);
}

void GaugeTacho::_drawDefaultForeground(CairoSurface& surface, double redlineWarningThreshold, double redline,
	double radialSectionLength, colour& normalColour, colour& redlineWarningThresholdColour, colour& redlineColour)
{
	cairo_t* cr = surface.getContext();

	double radius = (double)(_getWidth()) / 2.0;

/*
	// Draw indicator line.

	cairo_set_source_rgba(cr, indicatorLineColour.r, indicatorLineColour.g, indicatorLineColour.b, indicatorLineColour.a);

	cairo_set_line_width(cr, indicatorLineWidth);

	unsigned curSpeed = _speedoInstr.getSpeed();

	// Indicator angle starts from 20 at 0 degrees and increases clockwise to max speed.
	double indicatorAngle = M_PI * ((double)curSpeed - 20.0) / ((double)_maxSpeed - 20.0);

	// Rotate about the "dial centre".
	cairo_identity_matrix(cr);
	cairo_translate(cr, radius, radius);
	cairo_rotate(cr, indicatorAngle);
	cairo_translate(cr, -radius, -radius);

	// Define and draw line.
	cairo_move_to(cr, 0.0, radius);
	cairo_line_to(cr, indicatorLineLength, radius);
	cairo_stroke(cr);
*/
}
