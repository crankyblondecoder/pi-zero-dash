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
	double height =_getHeight();

	double oilCanWidth = width * 0.72;

	double oilCanLeft = (width - oilCanWidth) / 2.0;
	double oilCanRight = oilCanLeft + oilCanWidth;

	double oilCanBottom = (height / 2.0) + oilCanWidth * 0.2;

	double oilCanStrokeWidth = width / 20.0;

	if(_lastOilTemp < _lowTempThreshold)
	{
		cairo_set_source_rgba(cr, _tempColdBackgroundColour.r, _tempColdBackgroundColour.g, _tempColdBackgroundColour.b,
			_tempColdBackgroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		cairo_set_source_rgba(cr, _tempColdStrokeColour.r, _tempColdStrokeColour.g, _tempColdStrokeColour.b,
			_tempColdStrokeColour.a);
	}
	else if(_lastOilTemp < _highTempThreshold)
	{
		cairo_set_source_rgba(cr, _offStrokeColour.r, _offStrokeColour.g, _offStrokeColour.b, _offStrokeColour.a);
	}
	else
	{
		cairo_set_source_rgba(cr, _tempHighBackgroundColour.r, _tempHighBackgroundColour.g, _tempHighBackgroundColour.b,
			_tempHighBackgroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		cairo_set_source_rgba(cr, _tempHighStrokeColour.r, _tempHighStrokeColour.g, _tempHighStrokeColour.b,
			_tempHighStrokeColour.a);
	}

	cairo_set_line_width(cr, oilCanStrokeWidth);

	_drawOilCanPath(cr, oilCanLeft, oilCanRight, oilCanBottom, oilCanStrokeWidth, false);
	cairo_stroke(cr);

	_drawThermometerPath(cr, width * 0.37, width * 0.49, height * 0.2, height * 0.59);
	cairo_fill(cr);
}

bool GaugeOilTemperature_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_oilTempInstr)
	{
		double curOilTemp = _oilTempInstr.getOilTemperature();

		int curSection = 2;
		if(curOilTemp < _highTempThreshold) curSection--;
		if(curOilTemp < _lowTempThreshold) curSection--;

		int lastSection = 2;
		if(_lastOilTemp < _highTempThreshold) lastSection--;
		if(_lastOilTemp < _lowTempThreshold) lastSection--;

		return lastSection != curSection;
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