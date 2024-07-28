#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

#include "Gauge.hpp"

using namespace piZeroDash;

Gauge::~Gauge()
{
	delete[] _instruments;
}

Gauge::Gauge(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Visual(globalPositionX, globalPositionY, width, height, true, true)

{
	_instruments = new Instrument*[NUM_INSTRUMENTS];
	_instrumentCount = 0;
}

void Gauge::scan()
{
	// Latch instruments and if any have changed draw the foreground;
	bool draw = false;

	for(unsigned index = 0; index < _instrumentCount; index++)
	{
		// All instruments for a gauge need to be latched per scan.
		bool drawForeground = _instruments[index] -> latch() && _requiresDrawForeground(_instruments[index]);

		draw = draw || drawForeground;
	}

	if(!_firstForegroundDrawn || draw)
	{
		_drawForeground();

		_firstForegroundDrawn = true;
	}

	_composeForegroundToDisplay();
}

void Gauge::_addInstrument(Instrument* instrument)
{
	_instruments[_instrumentCount++] = instrument;
}

void Gauge::_drawDefaultBoxPath(cairo_t* cr, double cornerRadius, double left, double right, double top, double bottom)
{
	cairo_new_sub_path(cr);
	cairo_arc(cr, left + cornerRadius, top + cornerRadius, cornerRadius, M_PI, 1.5 * M_PI);
	cairo_line_to(cr, right - cornerRadius, top);
	cairo_arc(cr, right - cornerRadius, top + cornerRadius, cornerRadius, 1.5 * M_PI, 2.0 * M_PI);
	cairo_line_to(cr, right, bottom - cornerRadius);
	cairo_arc(cr, right - cornerRadius, bottom - cornerRadius, cornerRadius, 0.0, 0.5 * M_PI);
	cairo_line_to(cr, left - cornerRadius, bottom);
	cairo_arc(cr, left + cornerRadius, bottom - cornerRadius, cornerRadius, 0.5 * M_PI, M_PI);
	cairo_close_path(cr);
}

void Gauge::_drawBoxPath(cairo_t* cr, double topLeftRadius, double topRightRadius, double bottomLeftRadius,
	double bottomRightRadius, double left, double right, double top, double bottom)
{
	cairo_new_sub_path(cr);

	if(topLeftRadius == 0.0)
	{
		cairo_move_to(cr, left, top);
	}
	else
	{
		cairo_arc(cr, left + topLeftRadius, top + topLeftRadius, topLeftRadius, M_PI, 1.5 * M_PI);
	}

	cairo_line_to(cr, right - topRightRadius, top);

	if(topRightRadius != 0.0)
	{
		cairo_arc(cr, right - topRightRadius, top + topRightRadius, topRightRadius, 1.5 * M_PI, 2.0 * M_PI);
	}

	cairo_line_to(cr, right, bottom - bottomRightRadius);

	if(bottomRightRadius != 0.0)
	{
		cairo_arc(cr, right - bottomRightRadius, bottom - bottomRightRadius, bottomRightRadius, 0.0, 0.5 * M_PI);
	}

	cairo_line_to(cr, left - bottomLeftRadius, bottom);

	if(bottomRightRadius != 0.0)
	{
		cairo_arc(cr, left + bottomRightRadius, bottom - bottomRightRadius, bottomLeftRadius, 0.5 * M_PI, M_PI);
	}

	cairo_close_path(cr);
}

void Gauge::_drawThermometerPath(cairo_t* cr, double left, double right, double top, double bottom)
{
	double halfWidth = (right - left) / 2.0;
	double quarterWidth = halfWidth / 2.0;

	// Draw bulb at bottom

	double bulbRadius = quarterWidth * 1.5;

	double cutoutAngle = atan((quarterWidth / 2) / bulbRadius);

	cairo_new_sub_path(cr);

	cairo_arc(cr, left + bulbRadius, bottom - bulbRadius, bulbRadius, (-M_PI / 2.0) + cutoutAngle, (M_PI * 1.5) - cutoutAngle);

	// Draw rest of body.

	// The radius of the body section ends.
	double bodyRadius = quarterWidth / 2.0;

	double curX;
	double curY;
	cairo_get_current_point(cr, &curX, &curY);

	// Left line to top.
	cairo_line_to(cr, curX, top + bodyRadius);

	// Arc at top.
	cairo_arc(cr, left + bulbRadius, top + bodyRadius, bodyRadius, M_PI, 2.0 * M_PI);

	// Right line first section.
	cairo_rel_line_to(cr, 0.0, quarterWidth);

	// First temp mark.
	cairo_rel_line_to(cr, halfWidth - bodyRadius, 0.0);

	cairo_get_current_point(cr, &curX, &curY);
	cairo_arc(cr, curX, curY + bodyRadius, bodyRadius, -M_PI / 2.0, M_PI / 2.0);

	cairo_rel_line_to(cr, -halfWidth + bodyRadius, 0.0);

	// Right line second section.
	cairo_rel_line_to(cr, 0.0, quarterWidth);

	// Second temp mark.
	cairo_rel_line_to(cr, halfWidth - bodyRadius, 0.0);

	cairo_get_current_point(cr, &curX, &curY);
	cairo_arc(cr, curX, curY + bodyRadius, bodyRadius, -M_PI / 2.0, M_PI / 2.0);

	cairo_rel_line_to(cr, -halfWidth + bodyRadius, 0.0);

	// Right line third section.
	cairo_rel_line_to(cr, 0.0, quarterWidth);

	// Third temp mark.
	cairo_rel_line_to(cr, halfWidth - bodyRadius, 0.0);

	cairo_get_current_point(cr, &curX, &curY);
	cairo_arc(cr, curX, curY + bodyRadius, bodyRadius, -M_PI / 2.0, M_PI / 2.0);

	cairo_rel_line_to(cr, -halfWidth + bodyRadius, 0.0);

	// Final line section.
	cairo_close_path(cr);
}

void Gauge::_drawOilCanPath(cairo_t* cr, double left, double right, double bottom, double strokeWidth, bool drawLidHandle)
{
	double oilCanWidth = right - left;

	// Draw oil can body and spout.

	double bodyLeft = left + oilCanWidth * 0.2;

	cairo_move_to(cr, bodyLeft, bottom);
	cairo_rel_line_to(cr, oilCanWidth * 0.45, 0.0);
	cairo_rel_line_to(cr, oilCanWidth * 0.3, -oilCanWidth * 0.4);

	double spoutKinkX;
	double spoutKinkY;
	cairo_get_current_point(cr, &spoutKinkX, &spoutKinkY);

	// Start at the spout end kink.
	cairo_move_to(cr, spoutKinkX + oilCanWidth * 0.09, spoutKinkY + oilCanWidth * 0.07);
	cairo_line_to(cr, spoutKinkX, spoutKinkY);
	cairo_rel_line_to(cr, -oilCanWidth * 0.37, oilCanWidth * 0.2);
	cairo_rel_line_to(cr, -oilCanWidth * 0.06, -oilCanWidth * 0.06);

	double lidEndX;
	double lidEndY;
	cairo_get_current_point(cr, &lidEndX, &lidEndY);

	cairo_line_to(cr, bodyLeft, lidEndY);
	cairo_line_to(cr, bodyLeft, bottom + strokeWidth / 2.0);

	// Draw spout end drip.
	cairo_move_to(cr, spoutKinkX + oilCanWidth * 0.09, spoutKinkY + oilCanWidth * 0.07);
	cairo_rel_move_to(cr, 0.0, oilCanWidth * 0.09);
	cairo_rel_line_to(cr, 0.0, oilCanWidth * 0.11);

	if(drawLidHandle)
	{
		// Draw lid "T".
		cairo_move_to(cr, (lidEndX + bodyLeft) / 2.0, lidEndY);
		cairo_rel_line_to(cr, 0.0, -oilCanWidth * 0.12);
		cairo_rel_line_to(cr, -oilCanWidth * 0.1, 0.0);
		cairo_rel_line_to(cr, oilCanWidth * 0.2, 0.0);
	}

	// Draw handle.
	cairo_move_to(cr, bodyLeft, bottom - oilCanWidth * 0.14);
	cairo_rel_line_to(cr, 0.0, -oilCanWidth * 0.18);
	cairo_rel_line_to(cr, -oilCanWidth * 0.2, -oilCanWidth * 0.05);
	cairo_rel_line_to(cr, -oilCanWidth * 0.03, oilCanWidth * 0.12);
	cairo_close_path(cr);
}

void Gauge::_drawFuelLevelSymbol(cairo_t* cr, double left, double right, double top, double bottom, double strokeWidth,
	colour symbolColour)
{
	cairo_set_source_rgba(cr, symbolColour.r, symbolColour.g, symbolColour.b, symbolColour.a);
	cairo_set_line_width(cr, strokeWidth);

	double width = right - left;
	double height = bottom - top;

	double bowserWidth = width * 0.66;
	double bowserHeight = height;
	double bowserRight = left + bowserWidth;
	double bowserCornerRadius = bowserWidth * 0.1;

	cairo_move_to(cr, left, top + bowserCornerRadius);
	cairo_arc(cr, left + bowserCornerRadius, top + bowserCornerRadius, bowserCornerRadius, M_PI, M_PI * 1.5);
	cairo_rel_line_to(cr, bowserWidth - bowserCornerRadius * 2.0, 0.0);
	cairo_arc(cr, bowserRight - bowserCornerRadius, top + bowserCornerRadius, bowserCornerRadius, -M_PI / 2.0, 0.0);
	cairo_rel_line_to(cr, 0.0, bowserHeight - bowserCornerRadius);
	cairo_rel_line_to(cr, -bowserWidth, 0.0);
	cairo_close_path(cr);

	double bowserWindowOffset = bowserWidth * 0.1;
	double bowserWindowWidth = bowserWidth * 0.8;
	double bowserWindowHeight = bowserWindowWidth * 0.8;

	cairo_move_to(cr, left + bowserWindowOffset, top + bowserWindowOffset);
	cairo_rel_line_to(cr, 0.0, bowserWindowHeight);
	cairo_rel_line_to(cr, bowserWindowWidth, 0.0);
	cairo_rel_line_to(cr, 0.0, -bowserWindowHeight);
	cairo_close_path(cr);

	cairo_fill(cr);

	// Draw the pipe and nozzle.

	double hoseCurveRadius = (right - bowserRight - strokeWidth) / 3.0;
	double hoseStartCurveCentreY = top + bowserWindowOffset + bowserWindowHeight * 1.1 + hoseCurveRadius;

	cairo_arc(cr, bowserRight, hoseStartCurveCentreY, hoseCurveRadius, -M_PI / 2.0, 0.0);
	cairo_rel_line_to(cr, 0.0, bowserHeight * 0.25);

	double curX;
	double curY;
	cairo_get_current_point(cr, &curX, &curY);

	cairo_arc_negative(cr, curX + hoseCurveRadius, curY, hoseCurveRadius, M_PI, 0.0);
	cairo_rel_line_to(cr, 0.0, -bowserHeight * 0.55);
	cairo_rel_line_to(cr, -hoseCurveRadius, -hoseCurveRadius * 2.5);

	cairo_get_current_point(cr, &curX, &curY);

	cairo_stroke(cr);

	cairo_move_to(cr, curX, curY);
	cairo_rel_line_to(cr, hoseCurveRadius * 2.0, hoseCurveRadius * 2.5);

	cairo_stroke(cr);
}

void Gauge::_drawTurboPath(cairo_t* cr, double left, double right, double top, double bottom)
{
	double width = right - left;
	double height = bottom - top;

	double centreX = left + width / 2.0;
	double centreY = top + height / 2.0;

	double outerRadius = height * 0.5;

	double innerRadius = outerRadius * 0.5;

	// Outer snail.

	cairo_move_to(cr, right, top);
	cairo_line_to(cr, centreX, top);
	cairo_arc_negative(cr, centreX, centreY, outerRadius, M_PI * 1.5, 0.0);
	cairo_rel_line_to(cr, 0.0, -height * 0.1);
	cairo_rel_line_to(cr, -outerRadius * 0.2, 0.0);

	double curX;
	double curY;
	cairo_get_current_point(cr, &curX, &curY);

	cairo_line_to(cr, right, curY);
	cairo_close_path(cr);

	// Intake (inner).

	cairo_new_sub_path(cr);
	cairo_arc(cr, centreX, centreY, innerRadius, 0.0, M_PI * 2.0);

	double deg45_comp = 0.70710678 * innerRadius;

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, 0.0, -innerRadius);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, deg45_comp, -deg45_comp);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, innerRadius, 0.0);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, deg45_comp, deg45_comp);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, 0.0, innerRadius);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, -deg45_comp, deg45_comp);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, -innerRadius, 0.0);

	cairo_move_to(cr, centreX, centreY);
	cairo_rel_line_to(cr, -deg45_comp, -deg45_comp);
}