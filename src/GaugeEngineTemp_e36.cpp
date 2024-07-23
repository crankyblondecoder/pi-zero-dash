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
	int _lastEngineTemp = _engineTempInstr.getEngineTemp();

	cairo_t* cr = surface.getContext();

	cairo_identity_matrix(cr);

	double width = (double)(_getWidth());
	double height = (double)(_getHeight());

	// Draw box.

	if(_lastEngineTemp <= _coldLimitTemp)
	{
		cairo_set_source_rgba(cr, _boxColourCold.r, _boxColourCold.g, _boxColourCold.b, _boxColourCold.a);
	}
	else if(_lastEngineTemp <= _normalLimitTemp)
	{
		cairo_set_source_rgba(cr, _boxColourNormal.r, _boxColourNormal.g, _boxColourNormal.b, _boxColourNormal.a);
	}
	else
	{
		cairo_set_source_rgba(cr, _boxColourHot.r, _boxColourHot.g, _boxColourHot.b, _boxColourHot.a);
	}

	_drawDefaultBoxPath(cr, height / 4.0, 0.0, width, 0.0, height);

	cairo_fill(cr);

	// Draw temperature symbol.

	double thermometerWidth = width * 0.1;
	double thermometerRight = width * 0.25;
	double thermometerLeft = thermometerRight - thermometerWidth;
	double thermometerBottom = height * 0.7;

	_drawThermometerPath(cr, thermometerLeft, thermometerRight, height * 0.2, height * 0.7);

	cairo_set_source_rgba(cr, _fontColour.r, _fontColour.g, _fontColour.b, _fontColour.a);
	cairo_fill(cr);

	double waveRadius = thermometerWidth * 0.65;
	double sliceAngle = atan((thermometerWidth / 2.0) / waveRadius);
	double arcStartAngle = (M_PI / 2.0) - sliceAngle;
	double arcEndAngle = (M_PI / 2.0) + sliceAngle;

	cairo_set_line_width(cr, thermometerWidth / 4.0);

	double arcCentreX = thermometerLeft - thermometerWidth * 0.625;
	double arcCentreY = thermometerBottom - height * 0.05;

	cairo_arc_negative(cr, arcCentreX, arcCentreY, waveRadius, arcEndAngle, arcStartAngle);
	cairo_arc_negative(cr, arcCentreX + thermometerWidth, arcCentreY, waveRadius, arcEndAngle, arcStartAngle);
	cairo_arc_negative(cr, arcCentreX + thermometerWidth * 2.0, arcCentreY, waveRadius, arcEndAngle, arcStartAngle);
	cairo_stroke(cr);

	// Draw text.

	double fontSize = height / 2.7;

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, fontSize);

	cairo_text_extents_t textExtents;

	string numberText = to_string(_lastEngineTemp) + "°C";
	string extentString;

	// Use a fixed number string to get text extents. This is so the number doesn't jump around.
	if(_lastEngineTemp <= -10)
	{
		extentString = "-55";
	}
	else if(_lastEngineTemp < 0)
	{
		extentString = "-5";
	}
	else if(_lastEngineTemp < 10)
	{
		extentString = "5";
	}
	else if(_lastEngineTemp < 100)
	{
		extentString = "55";
	}
	else
	{
		extentString = "555";
	}

	extentString += "°C";

	cairo_text_extents(cr,  extentString.c_str(), &textExtents);

	cairo_set_source_rgba(cr, _fontColour.r, _fontColour.g, _fontColour.b, _fontColour.a);

	double top = (height - textExtents.height) / 2.0;
	double left = thermometerRight + (width - thermometerRight - textExtents.width) / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, numberText.c_str());
}

bool GaugeEngineTemp_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_engineTempInstr)
	{
		return _lastEngineTemp != _engineTempInstr.getEngineTemp();
	}

	return false;
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