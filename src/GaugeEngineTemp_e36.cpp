#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeEngineTemp_e36.hpp"

using namespace piZeroDash;

GaugeEngineTemp_e36::~GaugeEngineTemp_e36()
{
}

GaugeEngineTemp_e36::GaugeEngineTemp_e36(int minTemp, int maxTemp, int coldLimitTemp, int normalLimitTemp, int globalPositionX,
	int globalPositionY, unsigned width, unsigned height) : Gauge(globalPositionX, globalPositionY, width, height)
{
	_minTemp = minTemp;
	_maxTemp = maxTemp;
	_coldLimitTemp = coldLimitTemp;
	_normalLimitTemp = normalLimitTemp;

	_addInstrument(&_engineTempInstr);
}

void GaugeEngineTemp_e36::_drawBackground(CairoSurface& surface)
{
	// Doesn't have a background because the box colour can change.
}

void GaugeEngineTemp_e36::_drawForeground(CairoSurface& surface)
{
	int curTemp = _engineTempInstr.getEngineTemp();

	cairo_t* cr = surface.getContext();

	cairo_identity_matrix(cr);

	double width = (double)(_getWidth());
	double height = (double)(_getHeight());

	// Draw box.

	if(curTemp <= _coldLimitTemp)
	{
		cairo_set_source_rgba(cr, _boxColourCold.r, _boxColourCold.g, _boxColourCold.b, _boxColourCold.a);
	}
	else if(curTemp <= _normalLimitTemp)
	{
		cairo_set_source_rgba(cr, _boxColourNormal.r, _boxColourNormal.g, _boxColourNormal.b, _boxColourNormal.a);
	}
	else
	{
		cairo_set_source_rgba(cr, _boxColourHot.r, _boxColourHot.g, _boxColourHot.b, _boxColourHot.a);
	}

	_drawDefaultBoxPath(cr, height / 4.0, 0.0, width, 0.0, height);

	cairo_fill(cr);

	// Draw text.

	double fontSize = height / 2.0;

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, fontSize);

	cairo_text_extents_t textExtents;

	string numberText = to_string(curTemp) + "°C";
	string extentString;

	// Use a fixed number string to get text extents. This is so the number doesn't jump around.
	if(curTemp <= -10)
	{
		extentString = "-55°C";
	}
	else if(curTemp < 0)
	{
		extentString = "-5°C";
	}
	else if(curTemp < 10)
	{
		extentString = "5°C";
	}
	else if(curTemp < 100)
	{
		extentString = "55°C";
	}
	else
	{
		extentString = "555°C";
	}

	cairo_text_extents(cr,  extentString.c_str(), &textExtents);

	cairo_set_source_rgba(cr, _fontColour.r, _fontColour.g, _fontColour.b, _fontColour.a);

	double top = (height - textExtents.height) / 2.0;
	double left = (width - textExtents.width) / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, numberText.c_str());
}

void GaugeEngineTemp_e36::test()
{
	// Just use the low beam intrument for all tests.
	_engineTempInstr.test(_minTemp, _maxTemp);
}

bool GaugeEngineTemp_e36::inTestMode()
{
	return _engineTempInstr.inTestMode();
}