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
	double preciseSpeedBackgroundWidth, double preciseSpeedBackgroundHeight, string unitsLabelText)
{
	_setStandardProperties(20, _getMaxSpeed(), 10, true, true, true, markedSpeedFontSize,
		markedSpeedFontColour, markedSpeedFontDecimalPlaces, lineLength, majorLineWidth, minorLineWidth, lineStartOffset,
		majorLineColour, minorLineColour, M_PI, 2.0 * M_PI);

	_lineLength = lineLength;
	_lineStartOffset = lineStartOffset;

	_markedSpeedFontSize = markedSpeedFontSize;
	_markedSpeedFontColour = markedSpeedFontColour;

	_preciseSpeedBackgroundColour = preciseSpeedBackgroundColour;
	_preciseSpeedBackgroundWidth = preciseSpeedBackgroundWidth;
	_preciseSpeedBackgroundHeight = preciseSpeedBackgroundHeight;

	_unitsLabelText = unitsLabelText;
}

void GaugeSpeedo::_drawDefaultBackground(CairoSurface& surface)
{
	GaugeDial::_drawDefaultBackground(surface);

	_drawStandardPreciseBoxBackground(surface, _preciseSpeedBackgroundWidth, _preciseSpeedBackgroundHeight,
		_preciseSpeedBackgroundColour);

		// Draw the units label.

	cairo_t* cr = surface.getContext();

	cairo_identity_matrix(cr);

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, _markedSpeedFontSize / 2.0);

	cairo_text_extents_t textExtents;

	cairo_text_extents(cr, _unitsLabelText.c_str(), &textExtents);

	cairo_set_source_rgba(cr, _markedSpeedFontColour.r, _markedSpeedFontColour.g, _markedSpeedFontColour.b,
		_markedSpeedFontColour.a);

	double top = _getDialCentreY() - _getRadius() + _lineStartOffset + _lineLength * 1.4 + _markedSpeedFontSize;
	double left = _getDialCentreX() - textExtents.width / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, _unitsLabelText.c_str());
}

void GaugeSpeedo::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
	colour& indicatorLineColour, double preciseSpeedFontSize, colour& preciseSpeedFontColour)
{
	_lastSpeed = _speedoInstr.getSpeed();

	_drawStandardIndicatorLine(surface, (double) _lastSpeed, indicatorLineLength, indicatorLineWidth, indicatorLineColour);

	_drawStandardPreciseBoxForeground(surface, _lastSpeed, 0, "", preciseSpeedFontSize, preciseSpeedFontColour);
}

bool GaugeSpeedo::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_speedoInstr)
	{
		return _lastSpeed != _speedoInstr.getSpeed();
	}

	return false;
}
