#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeLowOilPressure_e36.hpp"

using namespace piZeroDash;

GaugeLowOilPressure_e36::~GaugeLowOilPressure_e36()
{
}

GaugeLowOilPressure_e36::GaugeLowOilPressure_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_oilPressureInstr);
}

void GaugeLowOilPressure_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeLowOilPressure_e36::_drawForeground(CairoSurface& surface)
{
	_lastOilPressure = _oilPressureInstr.getOilPressure();

	cairo_t* cr = surface.getContext();

	double width = _getWidth();

	if(_lastOilPressure < _lowPressureThreshold)
	{
		cairo_set_source_rgba(cr, _onForegroundColour.r, _onForegroundColour.g, _onForegroundColour.b, _onForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawLowOilPressureOutline(cr, width / 20.0, _onStrokeColour);
	}
	else
	{
		__drawLowOilPressureOutline(cr, width / 20.0, _offStrokeColour);
	}
}

bool GaugeLowOilPressure_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_oilPressureInstr)
	{
		return (_lastOilPressure < _lowPressureThreshold) != (_oilPressureInstr.getOilPressure() < _lowPressureThreshold);
	}

	return false;
}

void GaugeLowOilPressure_e36::__drawLowOilPressureOutline(cairo_t* cr, double strokeWidth, colour& strokeColour)
{
	cairo_set_source_rgba(cr, strokeColour.r, strokeColour.g, strokeColour.b, strokeColour.a);
	cairo_set_line_width(cr, strokeWidth);

	double width = _getWidth();
	double height =_getHeight();

	double oilCanWidth = width * 0.72;

	double oilCanLeft = (width - oilCanWidth) / 2.0;

	double oilCanBottom = (height / 2.0) + oilCanWidth * 0.2;

	// Draw oil can body and spout.

	double bodyLeft = oilCanLeft + oilCanWidth * 0.2;

	cairo_move_to(cr, bodyLeft, oilCanBottom);
	cairo_rel_line_to(cr, oilCanWidth * 0.45, 0.0);
	cairo_rel_line_to(cr, oilCanWidth * 0.3, -oilCanWidth * 0.4);

	double spoutKinkX;
	double spoutKinkY;
	cairo_get_current_point(cr, &spoutKinkX, &spoutKinkY);

	// Don't want the spout end to be a line join.
	cairo_stroke(cr);

	// Start at the spout end kink.
	cairo_move_to(cr, spoutKinkX + oilCanWidth * 0.09, spoutKinkY + oilCanWidth * 0.07);
	cairo_line_to(cr, spoutKinkX, spoutKinkY);
	cairo_rel_line_to(cr, -oilCanWidth * 0.37, oilCanWidth * 0.2);
	cairo_rel_line_to(cr, -oilCanWidth * 0.06, -oilCanWidth * 0.06);

	double lidEndX;
	double lidEndY;
	cairo_get_current_point(cr, &lidEndX, &lidEndY);

	cairo_line_to(cr, bodyLeft, lidEndY);
	cairo_line_to(cr, bodyLeft, oilCanBottom + strokeWidth / 2.0);

	cairo_stroke(cr);

	// Draw spout end drip.
	cairo_move_to(cr, spoutKinkX + oilCanWidth * 0.09, spoutKinkY + oilCanWidth * 0.07);
	cairo_rel_move_to(cr, 0.0, oilCanWidth * 0.09);
	cairo_rel_line_to(cr, 0.0, oilCanWidth * 0.11);

	cairo_stroke(cr);

	// Draw lid "T".
	cairo_move_to(cr, (lidEndX + bodyLeft) / 2.0, lidEndY);
	cairo_rel_line_to(cr, 0.0, -oilCanWidth * 0.12);
	cairo_rel_line_to(cr, -oilCanWidth * 0.1, 0.0);
	cairo_rel_line_to(cr, oilCanWidth * 0.2, 0.0);
	cairo_stroke(cr);

	// Draw handle.
	cairo_move_to(cr, bodyLeft, oilCanBottom - oilCanWidth * 0.14);
	cairo_rel_line_to(cr, 0.0, -oilCanWidth * 0.18);
	cairo_rel_line_to(cr, -oilCanWidth * 0.2, -oilCanWidth * 0.05);
	cairo_rel_line_to(cr, -oilCanWidth * 0.03, oilCanWidth * 0.12);
	cairo_close_path(cr);
	cairo_stroke(cr);
}

void GaugeLowOilPressure_e36::test()
{
	_oilPressureInstr.test(400);
}

bool GaugeLowOilPressure_e36::inTestMode()
{
	return _oilPressureInstr.inTestMode();
}