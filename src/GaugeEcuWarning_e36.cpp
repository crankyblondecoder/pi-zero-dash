#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeEcuWarning_e36.hpp"

using namespace piZeroDash;

GaugeEcuWarning_e36::~GaugeEcuWarning_e36()
{
}

GaugeEcuWarning_e36::GaugeEcuWarning_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_ecuWarningOnOffInstr);
}

void GaugeEcuWarning_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeEcuWarning_e36::_drawForeground(CairoSurface& surface)
{
	_lastEcuWarn = _ecuWarningOnOffInstr.getOnOffState();

	cairo_t* cr = surface.getContext();

	double width = _getWidth();
	double height = _getHeight();

	if(_lastEcuWarn)
	{
		cairo_set_source_rgba(cr, _onForegroundColour.r, _onForegroundColour.g, _onForegroundColour.b, _onForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		cairo_set_source_rgba(cr, _onStrokeColour.r, _onStrokeColour.g, _onStrokeColour.b, _onStrokeColour.a);
	}
	else
	{
		cairo_set_source_rgba(cr, _offStrokeColour.r, _offStrokeColour.g, _offStrokeColour.b, _offStrokeColour.a);
	}

	double fontSize = width * 0.4;

	string text = "ECU";

	cairo_select_font_face(cr, "DejaVu Sans Condensed", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, fontSize);

	cairo_text_extents_t textExtents;

	cairo_text_extents(cr,  text.c_str(), &textExtents);

	// Centre text on light.
	double top = (height - textExtents.height) / 2.0;
	double left = (width - textExtents.width) / 2.0;

	cairo_move_to(cr, left - textExtents.x_bearing, top - textExtents.y_bearing);
	cairo_show_text(cr, text.c_str());
}

bool GaugeEcuWarning_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_ecuWarningOnOffInstr)
	{
		return _lastEcuWarn != _ecuWarningOnOffInstr.getOnOffState();
	}

	return false;
}

void GaugeEcuWarning_e36::test()
{
	_ecuWarningOnOffInstr.test();
}

bool GaugeEcuWarning_e36::inTestMode()
{
	return _ecuWarningOnOffInstr.inTestMode();
}