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

	_drawOilCanPath(cr, oilCanLeft, oilCanLeft + oilCanWidth, oilCanBottom, strokeWidth, true);

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