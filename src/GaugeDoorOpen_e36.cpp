#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeDoorOpen_e36.hpp"

using namespace piZeroDash;

GaugeDoorOpen_e36::~GaugeDoorOpen_e36()
{
}

GaugeDoorOpen_e36::GaugeDoorOpen_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_doorOpenOnOffInstr);
}

void GaugeDoorOpen_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeDoorOpen_e36::_drawForeground(CairoSurface& surface)
{
	bool doorOpen = _doorOpenOnOffInstr.getOnOffState();

	cairo_t* cr = surface.getContext();

	double width = _getWidth();

	if(doorOpen)
	{
		cairo_set_source_rgba(cr, _onForegroundColour.r, _onForegroundColour.g, _onForegroundColour.b, _onForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawDoorOpenOutline(cr, width / 20.0, _onStrokeColour);
	}
	else
	{
		__drawDoorOpenOutline(cr, width / 20.0, _offStrokeColour);
	}
}

void GaugeDoorOpen_e36::__drawDoorOpenOutline(cairo_t* cr, double strokeWidth, colour& strokeColour)
{
	cairo_set_source_rgba(cr, strokeColour.r, strokeColour.g, strokeColour.b, strokeColour.a);
	cairo_set_line_width(cr, strokeWidth);

	double width = _getWidth();
	double height =_getHeight();

	double carWidth = width * 0.4;
	double carHeight = height * 0.65;

	double carBoxLeft = (width - carWidth) / 2.0;
	double carBoxRight = carBoxLeft + carWidth;

	double carBoxTop = (height - carHeight) / 2.0;
	double carBoxBottom = carBoxTop + carHeight;

	double carBonnetBottom = carBoxTop + carHeight * 0.35;
	double carBootTop = carBoxBottom - carHeight * 0.15;

	double cornerRadius = carWidth * 0.15;
	double halfStrokeWidth = strokeWidth / 2.0;

	double doorWidth = carWidth * 0.5;

	// Draw bonnet.
	_drawBoxPath(cr, cornerRadius, cornerRadius, 0.0, 0.0, carBoxLeft, carBoxRight, carBoxTop, carBonnetBottom);
	cairo_fill(cr);

	// Draw boot.
	_drawBoxPath(cr, 0.0, 0.0, cornerRadius, cornerRadius, carBoxLeft, carBoxRight, carBootTop, carBoxBottom);
	cairo_fill(cr);

	// Draw lines between bonnet and boot.
	cairo_move_to(cr, carBoxLeft + halfStrokeWidth, carBonnetBottom);
	cairo_line_to(cr, carBoxLeft + halfStrokeWidth, carBootTop);
	cairo_move_to(cr, carBoxRight - halfStrokeWidth, carBonnetBottom);
	cairo_line_to(cr, carBoxRight - halfStrokeWidth, carBootTop);
	cairo_stroke(cr);

	// Draw doors.
	cairo_move_to(cr, carBoxLeft + halfStrokeWidth, carBonnetBottom);
	cairo_line_to(cr, carBoxLeft - doorWidth, carBonnetBottom + doorWidth);
	cairo_move_to(cr, carBoxRight - halfStrokeWidth, carBonnetBottom);
	cairo_line_to(cr, carBoxRight + doorWidth, carBonnetBottom + doorWidth);
	cairo_stroke(cr);

	// TODO ...
}

void GaugeDoorOpen_e36::test()
{
	_doorOpenOnOffInstr.test();
}

bool GaugeDoorOpen_e36::inTestMode()
{
	return _doorOpenOnOffInstr.inTestMode();
}