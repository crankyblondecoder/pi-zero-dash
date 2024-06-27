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

bounds GaugeSpeedo::__calcPreciseSpeedBoxBounds(double width, double height)
{
	double dialCentreX = _getDialCentreX();
	double dialCentreY = _getDialCentreY();

	struct bounds retBounds;

	retBounds.left = dialCentreX - width / 2.0;
	retBounds.right = retBounds.left + width;
	retBounds.top = dialCentreY - height / 2.0;
	retBounds.bottom = retBounds.top + height;

	return retBounds;
}

bounds GaugeSpeedo::_calcPreciseSpeedBoxBounds(double width, double height)
{
	return __calcPreciseSpeedBoxBounds(width, height);
}

void GaugeSpeedo::_drawDefaultBackground(CairoSurface& surface, double markedSpeedFontSize, colour& markedSpeedFontColour,
	unsigned markedSpeedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
	double lineStartOffset, colour& majorLineColour, colour& minorLineColour, colour& preciseSpeedBackgroundColour,
	double preciseSpeedBackgroundWidth, double preciseSpeedBackgroundHeight)
{
	cairo_t* cr = surface.getContext();

	GaugeDial::_drawDefaultBackground(surface, 20, _getMaxSpeed(), 10, true, true, true, markedSpeedFontSize,
		markedSpeedFontColour, markedSpeedFontDecimalPlaces, lineLength, majorLineWidth, minorLineWidth, lineStartOffset,
		majorLineColour, minorLineColour);

	// Draw precise speed background.
	cairo_identity_matrix(cr);

	cairo_set_source_rgba(cr, preciseSpeedBackgroundColour.r, preciseSpeedBackgroundColour.g, preciseSpeedBackgroundColour.b,
		preciseSpeedBackgroundColour.a);

	double cornerRadius = preciseSpeedBackgroundHeight / 4.0;

	struct bounds precSpeedBox = __calcPreciseSpeedBoxBounds(preciseSpeedBackgroundWidth, preciseSpeedBackgroundHeight);

	_drawDefaultBoxPath(cr, cornerRadius, precSpeedBox.left, precSpeedBox.right, precSpeedBox.top, precSpeedBox.bottom);

	cairo_fill(cr);
}

void GaugeSpeedo::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
	colour& indicatorLineColour, double preciseSpeedFontSize, colour& preciseSpeedFontColour)
{
	cairo_t* cr = surface.getContext();

	double radius = _getRadius();

	double dialCentreX = _getDialCentreX();
	double dialCentreY = _getDialCentreY();

	// Draw indicator line.

	cairo_set_source_rgba(cr, indicatorLineColour.r, indicatorLineColour.g, indicatorLineColour.b, indicatorLineColour.a);

	cairo_set_line_width(cr, indicatorLineWidth);

	unsigned curSpeed = _speedoInstr.getSpeed();

	// Indicator angle starts from 20 at 0 degrees and increases clockwise to max speed.
	double indicatorAngle = M_PI * ((double)curSpeed - 20.0) / ((double)_maxSpeed - 20.0);

	// Rotate about the "dial centre".
	cairo_identity_matrix(cr);
	cairo_translate(cr, dialCentreX, dialCentreY);
	cairo_rotate(cr, indicatorAngle);
	cairo_translate(cr, -dialCentreX, -dialCentreY);

	// Define and draw line.
	cairo_move_to(cr, dialCentreX - radius, dialCentreY);
	cairo_line_to(cr, dialCentreX - radius + indicatorLineLength, dialCentreY);
	cairo_stroke(cr);

	// Write current speed to precise speed box.

	cairo_identity_matrix(cr);

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, preciseSpeedFontSize);

	cairo_text_extents_t textExtents;

	string numberText = to_string(curSpeed);
	string extentString;

	// Use a fixed number string to get text extents. This is so the number doesn't jump around.
	if(curSpeed < 10)
	{
		extentString = "5";
	}
	else if(curSpeed < 100)
	{
		extentString = "55";
	}
	else
	{
		extentString = "555";
	}

	cairo_text_extents(cr,  extentString.c_str(), &textExtents);

	cairo_set_source_rgba(cr, preciseSpeedFontColour.r, preciseSpeedFontColour.g, preciseSpeedFontColour.b,
		preciseSpeedFontColour.a);

	double top = dialCentreX - textExtents.height / 2.0;
	double left = dialCentreY - textExtents.width / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, numberText.c_str());
}
