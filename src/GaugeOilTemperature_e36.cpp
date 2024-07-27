#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeOilTemperature_e36.hpp"

using namespace piZeroDash;

GaugeOilTemperature_e36::~GaugeOilTemperature_e36()
{
}

GaugeOilTemperature_e36::GaugeOilTemperature_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_oilTempInstr);
}

void GaugeOilTemperature_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeOilTemperature_e36::_drawForeground(CairoSurface& surface)
{
	_lastOilTemp = _oilTempInstr.getOilTemperature();

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

bool GaugeOilTemperature_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_oilTempInstr)
	{
		return (_lastOilTemp < _lowPressureThreshold) != (_oilTempInstr.getOilTemperature() < _lowPressureThreshold);
	}

	return false;
}

void GaugeOilTemperature_e36::test()
{
	_oilTempInstr.test(130);
}

bool GaugeOilTemperature_e36::inTestMode()
{
	return _oilTempInstr.inTestMode();
}