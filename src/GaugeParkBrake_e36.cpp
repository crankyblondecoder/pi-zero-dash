#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeParkBrake_e36.hpp"

using namespace piZeroDash;

GaugeParkBrake_e36::~GaugeParkBrake_e36()
{
}

GaugeParkBrake_e36::GaugeParkBrake_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_parkBrakeOnOffInstr);
}

void GaugeParkBrake_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeParkBrake_e36::_drawForeground(CairoSurface& surface)
{
	bool parkBrakeState = _parkBrakeOnOffInstr.getOnOffState();

	cairo_t* cr = surface.getContext();

	double width = _getWidth();

	if(parkBrakeState)
	{
		cairo_set_source_rgba(cr, _onForegroundColour.r, _onForegroundColour.g, _onForegroundColour.b, _onForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawParkBrakeOutline(cr, width / 20.0, _onStrokeColour);
	}
	else
	{
		__drawParkBrakeOutline(cr, width / 20.0, _offStrokeColour);
	}
}

void GaugeParkBrake_e36::__drawParkBrakeOutline(cairo_t* cr, double strokeWidth, colour& strokeColour)
{
	double width = _getWidth();
	double height =_getHeight();

	double halfHeight = height / 2.0;
	double halfWidth = width / 2.0;

	double innerCircleRadius = halfHeight * 0.55;
	double outerCircleRadius = halfHeight * 0.75;

	cairo_set_source_rgba(cr, strokeColour.r, strokeColour.g, strokeColour.b, strokeColour.a);
	cairo_set_line_width(cr, strokeWidth);

	// Draw main circle.
	cairo_new_sub_path(cr);
	cairo_arc(cr, halfWidth, halfHeight, innerCircleRadius, 0.0, 2.0 * M_PI);

	// Draw left part circle.
	cairo_new_sub_path(cr);
	cairo_arc(cr, halfWidth, halfHeight, outerCircleRadius, 0.75 * M_PI, 1.25 * M_PI);

	// Draw right part circle.
	cairo_new_sub_path(cr);
	cairo_arc(cr, halfWidth, halfHeight, outerCircleRadius, -0.25 * M_PI, 0.25 * M_PI);

	cairo_stroke(cr);

	// Draw text.

	double fontSize = height * 0.4;

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, fontSize);

	cairo_text_extents_t textExtents;

	string text = "P";

	cairo_text_extents(cr, text.c_str(), &textExtents);

	// Place text dead in the middle.
	double top = (height - textExtents.height) / 2.0;
	double left = (width - textExtents.width) / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, text.c_str());
}

void GaugeParkBrake_e36::test()
{
	_parkBrakeOnOffInstr.test();
}

bool GaugeParkBrake_e36::inTestMode()
{
	return _parkBrakeOnOffInstr.inTestMode();
}