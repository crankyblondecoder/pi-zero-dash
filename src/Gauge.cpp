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
	double quarterWidth = (right - left) / 4.0;

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
	cairo_line_to(cr, curX, top - bodyRadius);

	// Arc at top.
	cairo_arc(cr, 1.5 * quarterWidth, top + bodyRadius, bodyRadius, M_PI, 2.0 * M_PI);

	// TODO ...

}