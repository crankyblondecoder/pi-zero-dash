#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeTurnIndicator.hpp"

using namespace piZeroDash;

GaugeTurnIndicator::~GaugeTurnIndicator()
{
}

GaugeTurnIndicator::GaugeTurnIndicator(bool left, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Gauge(globalPositionX, globalPositionY, width, height)
{
	_left = left;
	_addInstrument(&_indicatorInstr);
}

void GaugeTurnIndicator::test()
{
	_indicatorInstr.test();
}

bool GaugeTurnIndicator::inTestMode()
{
	return _indicatorInstr.inTestMode();
}

void GaugeTurnIndicator::_drawDefaultBackground(CairoSurface& surface, colour& fillColour)
{
	cairo_t* cr = surface.getContext();

	cairo_set_source_rgba(cr, fillColour.r, fillColour.g, fillColour.b, fillColour.a);
	__drawDefaultPath(cr);
	cairo_fill(cr);
}

void GaugeTurnIndicator::_drawDefaultForeground(CairoSurface& surface, colour& fillColour)
{
	_lastIndicatorState = _indicatorInstr.getIndicatorState();

	if((_lastIndicatorState == InstrumentIndicator::IndicatorState::LEFT && _left) ||
		(_lastIndicatorState == InstrumentIndicator::IndicatorState::RIGHT && !_left) ||
		(_lastIndicatorState == InstrumentIndicator::IndicatorState::BOTH))
	{
		cairo_t* cr = surface.getContext();

		cairo_set_source_rgba(cr, fillColour.r, fillColour.g, fillColour.b, fillColour.a);
		__drawDefaultPath(cr);
		cairo_fill(cr);
	}
}

bool GaugeTurnIndicator::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_indicatorInstr)
	{
		return _lastIndicatorState != _indicatorInstr.getIndicatorState();
	}

	return false;
}

void GaugeTurnIndicator::__drawDefaultPath(cairo_t* cr)
{
	double width = _getWidth();
	double height = _getHeight();

	cairo_identity_matrix(cr);

	if(_left)
	{
		cairo_translate(cr, width, height);
		cairo_rotate(cr, M_PI);
	}

	double noseRadius = height / 14.0;
	double noseDiameter = noseRadius * 2.0;

	double shaftWidth = width * 0.35;
	double shaftHeight = height * 0.55;

	double distToShaftY = (height - shaftHeight) / 2.0;

	cairo_move_to(cr, 0.0, distToShaftY);
	cairo_rel_line_to(cr, 0.0, shaftHeight);
	cairo_rel_line_to(cr, shaftWidth, 0.0);
	cairo_rel_line_to(cr, 0.0, distToShaftY);
	cairo_rel_line_to(cr, noseDiameter, 0.0);

	double curX;
	double curY;

	cairo_get_current_point(cr, &curX, &curY);

	double arcCentreX = width - noseRadius;
	double arcCentreY = height / 2.0;

	double curToArcCentreX_dist = arcCentreX - curX;
	double curToArcCentreY_dist = curY - arcCentreY;

	double alpha = atan(curToArcCentreY_dist / curToArcCentreX_dist);

	double curToArcCentreDist = sqrt((curToArcCentreX_dist * curToArcCentreX_dist) + (curToArcCentreY_dist *
		curToArcCentreY_dist));

	double beta = acos(noseRadius / curToArcCentreDist);

	double gamma = M_PI - alpha - beta;

	double tangentX = arcCentreX + noseRadius * cos(gamma);
	double tangentY = arcCentreY + noseRadius * sin(gamma);

	cairo_line_to(cr, tangentX, tangentY);
	cairo_arc_negative(cr, arcCentreX, arcCentreY, noseRadius, gamma, -gamma);

	cairo_line_to(cr, curX, 0.0);
	cairo_rel_line_to(cr, -noseDiameter, 0.0);
	cairo_rel_line_to(cr, 0.0, distToShaftY);
	cairo_close_path(cr);
}
