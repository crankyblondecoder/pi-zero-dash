#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeHeadlight_e36.hpp"

using namespace piZeroDash;

GaugeHeadlight_e36::~GaugeHeadlight_e36()
{
}

GaugeHeadlight_e36::GaugeHeadlight_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeStdLight(width / 8.0, globalPositionX, globalPositionY, width, height)
{
	_addInstrument(&_headLightLowBeamInstr);
	_addInstrument(&_headLightHighBeamInstr);
}

void GaugeHeadlight_e36::_drawBackground(CairoSurface& surface)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, _backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	_drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeHeadlight_e36::_drawForeground(CairoSurface& surface)
{
	_lastLowBeam = _headLightLowBeamInstr.getOnOffState();
	_lastHighBeam = _headLightHighBeamInstr.getOnOffState();

	bool lowBeamState = _lastLowBeam;
	bool highBeamState;

	if(_headLightLowBeamInstr.inTestMode())
	{
		if(lowBeamState) _testLowHighBeamState++;

		lowBeamState = _testLowHighBeamState & 1;
		highBeamState = _testLowHighBeamState & 2;
	}
	else
	{
		highBeamState = _headLightHighBeamInstr.getOnOffState();
	}

	cairo_t* cr = surface.getContext();

	double width = _getWidth();

	if(highBeamState)
	{
		cairo_set_source_rgba(cr, _highBeamForegroundColour.r, _highBeamForegroundColour.g, _highBeamForegroundColour.b,
			_highBeamForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawHeadlightOutline(cr, width / 20.0, _onStrokeColour, 0.0);
	}
	else if(lowBeamState)
	{
		cairo_set_source_rgba(cr, _lowBeamForegroundColour.r, _lowBeamForegroundColour.g, _lowBeamForegroundColour.b,
			_lowBeamForegroundColour.a);

		_drawDefaultPath(cr);
		cairo_fill(cr);

		__drawHeadlightOutline(cr, width / 20.0, _onStrokeColour, M_PI * 10.0 / 180.0);
	}
	else
	{
		__drawHeadlightOutline(cr, width / 20.0, _offStrokeColour, 0.0);
	}
}

bool GaugeHeadlight_e36::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_headLightLowBeamInstr)
	{
		return _lastLowBeam != _headLightLowBeamInstr.getOnOffState();
	}

	if(instrument == &_headLightHighBeamInstr)
	{
		return _lastHighBeam != _headLightHighBeamInstr.getOnOffState();
	}

	return false;
}

void GaugeHeadlight_e36::__drawHeadlightOutline(cairo_t* cr, double strokeWidth, colour& strokeColour,
	double beamDownSlantAngle)
{
	double width = _getWidth();
	double height =_getHeight();

	double halfHeight = height / 2.0;
	double halfWidth = width / 2.0;
	double quarterWidth = width / 4.0;

	double lensHalfAngle = (15.0 / 180.0) * M_PI;
	double lensRadius = width * 0.8;
	double lensHalfHeight = lensRadius * sin(lensHalfAngle);
	double lensDepthCurveDelta = lensRadius - lensRadius * cos(lensHalfAngle);

	double beamDownDeltaY = quarterWidth * sin(beamDownSlantAngle);

	// Setup common context.
	cairo_set_source_rgba(cr, strokeColour.r, strokeColour.g, strokeColour.b, strokeColour.a);
	cairo_set_line_width(cr, strokeWidth);

	// Calculate 5 points on lense surface. From top to bottom.

	double lensPt3_x = halfWidth;
	double lensPt3_y = halfHeight;

	double lensPt1_x = lensPt3_x + lensDepthCurveDelta;
	double lensPt1_y = lensPt3_y - lensHalfHeight;

	double lensPt2_x = lensPt3_x + lensDepthCurveDelta / 2.0;
	double lensPt2_y = lensPt3_y - lensHalfHeight / 2.0;

	double lensPt4_x = lensPt2_x;
	double lensPt4_y = lensPt3_y + lensHalfHeight / 2.0;

	double lensPt5_x = lensPt1_x;
	double lensPt5_y = lensPt3_y + lensHalfHeight;

	// Draw light outline.
	cairo_move_to(cr, lensPt5_x, lensPt5_y);
	cairo_arc(cr, halfWidth + lensRadius, halfHeight, lensRadius, M_PI - lensHalfAngle, M_PI + lensHalfAngle);
	cairo_curve_to(cr, lensPt1_x + halfWidth * 0.8, lensPt1_y, lensPt1_x + halfWidth * 0.8, lensPt5_y, lensPt5_x, lensPt5_y);

	// Draw light beams.

	double lensToBeamGap = quarterWidth / 4.0;
	double beamEndX = quarterWidth * 0.8;

	cairo_move_to(cr, lensPt1_x - lensToBeamGap, lensPt1_y);
	cairo_line_to(cr, beamEndX, lensPt1_y + beamDownDeltaY);


	cairo_move_to(cr, lensPt2_x - lensToBeamGap, lensPt2_y);
	cairo_line_to(cr, beamEndX, lensPt2_y + beamDownDeltaY);

	cairo_move_to(cr, lensPt3_x - lensToBeamGap, lensPt3_y);
	cairo_line_to(cr, beamEndX, lensPt3_y + beamDownDeltaY);

	cairo_move_to(cr, lensPt4_x - lensToBeamGap, lensPt4_y);
	cairo_line_to(cr, beamEndX, lensPt4_y + beamDownDeltaY);

	cairo_move_to(cr, lensPt5_x - lensToBeamGap, lensPt5_y);
	cairo_line_to(cr, beamEndX, lensPt5_y + beamDownDeltaY);

	cairo_stroke(cr);
}

void GaugeHeadlight_e36::test()
{
	_testLowHighBeamState = 0;

	// Just use the low beam intrument for all tests.
	_headLightLowBeamInstr.test();
}

bool GaugeHeadlight_e36::inTestMode()
{
	return _headLightHighBeamInstr.inTestMode();
}