#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeLowVoltage_e36.hpp"

using namespace piZeroDash;

GaugeLowVoltage_e36::~GaugeLowVoltage_e36()
{
}

GaugeLowVoltage_e36::GaugeLowVoltage_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_voltageInstr);
}

void GaugeLowVoltage_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeLowVoltage_e36::_drawForeground(CairoSurface& surface)
{
	_lastVoltage = _voltageInstr.getVoltage();

	cairo_t* cr = surface.getContext();

	double width = _getWidth();

	if(_lastVoltage < _lowVoltageThreshold)
	{
		cairo_set_source_rgba(cr, _onForegroundColour.r, _onForegroundColour.g, _onForegroundColour.b, _onForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawLowVoltageOutline(cr, width / 20.0, _onStrokeColour);
	}
	else
	{
		__drawLowVoltageOutline(cr, width / 20.0, _offStrokeColour);
	}
}

bool GaugeLowVoltage_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_voltageInstr)
	{
		return (_lastVoltage < _lowVoltageThreshold) != (_voltageInstr.getVoltage() < _lowVoltageThreshold);
	}

	return false;
}


void GaugeLowVoltage_e36::__drawLowVoltageOutline(cairo_t* cr, double strokeWidth, colour& strokeColour)
{
	cairo_set_source_rgba(cr, strokeColour.r, strokeColour.g, strokeColour.b, strokeColour.a);
	cairo_set_line_width(cr, strokeWidth);

	double width = _getWidth();
	double height =_getHeight();

	double batteryWidth = width * 0.72;
	double batteryHeight = batteryWidth * 0.6;

	double batteryLeft = (width - batteryWidth) / 2.0;
	double batteryRight = batteryLeft + batteryWidth;

	double batteryTop = (height - batteryHeight) / 2.0;
	double batteryBottom = batteryTop + batteryHeight;

	// Draw battery main box.
	_drawBoxPath(cr, 0.0, 0.0, 0.0, 0.0, batteryLeft, batteryRight, batteryTop, batteryBottom);
	cairo_stroke(cr);

	// Draw terminal lines.

	double terminalLineLength = width * 0.15;
	double edgeDistToTerminal = terminalLineLength * 0.7;

	cairo_move_to(cr, batteryLeft + edgeDistToTerminal, batteryTop - strokeWidth);
	cairo_rel_line_to(cr, terminalLineLength, 0.0);

	cairo_move_to(cr, batteryRight - edgeDistToTerminal, batteryTop - strokeWidth);
	cairo_rel_line_to(cr, -terminalLineLength, 0.0);

	// Draw positive and negative symbols.

	cairo_move_to(cr, batteryLeft + edgeDistToTerminal, batteryTop + batteryHeight * 0.3);
	cairo_rel_line_to(cr, terminalLineLength, 0.0);

	cairo_move_to(cr, batteryRight - edgeDistToTerminal, batteryTop + batteryHeight * 0.3);
	cairo_rel_line_to(cr, -terminalLineLength, 0.0);

	cairo_move_to(cr, batteryRight - edgeDistToTerminal - terminalLineLength * 0.5,
		batteryTop + batteryHeight * 0.3 - terminalLineLength * 0.5);

	cairo_rel_line_to(cr, 0.0, terminalLineLength);

	cairo_stroke(cr);
}

void GaugeLowVoltage_e36::test()
{
	_voltageInstr.test(14.5);
}

bool GaugeLowVoltage_e36::inTestMode()
{
	return _voltageInstr.inTestMode();
}