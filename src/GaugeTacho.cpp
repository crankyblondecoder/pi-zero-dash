#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeTacho.hpp"

using namespace piZeroDash;

GaugeTacho::~GaugeTacho()
{
}

GaugeTacho::GaugeTacho(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, int globalPositionX,
	int globalPositionY, unsigned width, unsigned height) : GaugeDial(globalPositionX, globalPositionY, width, height)
{
	_maxRpm = maxRpm;
	_redlineRpm = redlineRpm;
	_redlineWarningRpm = redlineWarningRpm;

	_addInstrument(&_tachoInstr);
}

unsigned GaugeTacho::_getMaxRpm()
{
	return _maxRpm;
}

void GaugeTacho::test()
{
	_tachoInstr.test(_maxRpm);
}

bool GaugeTacho::inTestMode()
{
	return _tachoInstr.inTestMode();
}

void GaugeTacho::_drawDefaultBackground(CairoSurface& surface, double markedRpmFontSize, colour& markedRpmFontColour,
	unsigned markedRpmFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
	double lineStartOffset, colour& majorLineColour, colour& minorLineColour)
{
	_drawDefaultDialIndicatorBackground(surface, 0, _getMaxRpm() / 1000, 1, true, true, true, markedRpmFontSize,
		markedRpmFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour);
}

void GaugeTacho::_drawDefaultForeground(CairoSurface& surface, double radialSectionLength, colour& normalColour,
	colour& redlineWarningThresholdColour, colour& redlineColour)
{
	cairo_t* cr = surface.getContext();

	double width = _getWidth();
	double radius = width / 2.0;

	unsigned curRpm = _tachoInstr.getRpm();

	double degreesPerRpm = (M_PI / (double) _maxRpm);

	// Draw normal rpm range.

	unsigned normalEnd = curRpm > _redlineWarningRpm ? _redlineWarningRpm : curRpm;

	double normalArcAngle = degreesPerRpm * (double) normalEnd;

	cairo_identity_matrix(cr);

	cairo_set_source_rgba(cr, normalColour.r, normalColour.g, normalColour.b, normalColour.a);
	cairo_move_to(cr, 0.0, radius);
	cairo_line_to(cr, radialSectionLength, radius);
	cairo_arc(cr, radius, radius, radius - radialSectionLength, M_PI, M_PI + normalArcAngle);
	cairo_rel_line_to(cr, -radialSectionLength * cos(normalArcAngle), -radialSectionLength * sin(normalArcAngle));
	cairo_arc_negative(cr, radius, radius, radius, M_PI + normalArcAngle, M_PI);

	cairo_close_path(cr);
	cairo_fill(cr);

	if(curRpm > _redlineWarningRpm)
	{
		// Draw the redline warning rpm range.

		unsigned redlineWarningRpmSpan = (curRpm < _redlineRpm ? curRpm : _redlineRpm)- _redlineWarningRpm;

		double redlineWarningArcAngle = degreesPerRpm * (double) redlineWarningRpmSpan;
		double absRedlineWarningArcAngle = normalArcAngle + redlineWarningArcAngle;

		cairo_set_source_rgba(cr, redlineWarningThresholdColour.r, redlineWarningThresholdColour.g,
			redlineWarningThresholdColour.b, redlineWarningThresholdColour.a);

		cairo_new_sub_path(cr);
		cairo_arc(cr, radius, radius, radius - radialSectionLength, M_PI + normalArcAngle, M_PI + absRedlineWarningArcAngle);
		cairo_rel_line_to(cr, -radialSectionLength * cos(absRedlineWarningArcAngle),
			-radialSectionLength * sin(absRedlineWarningArcAngle));
		cairo_arc_negative(cr, radius, radius, radius, M_PI + absRedlineWarningArcAngle, M_PI + normalArcAngle);

		cairo_close_path(cr);
		cairo_fill(cr);
	}

	if(curRpm > _redlineRpm)
	{
		// Draw the redline rpm range.

		unsigned redlineRpmSpan = curRpm - _redlineRpm;

		double redlineArcAngle = degreesPerRpm * (double) redlineRpmSpan;
		double absRedlineArcStartAngle = degreesPerRpm * (double) _redlineRpm;
		double absRedlineArcAngle = absRedlineArcStartAngle + redlineArcAngle;

		cairo_set_source_rgba(cr, redlineColour.r, redlineColour.g, redlineColour.b, redlineColour.a);

		cairo_new_sub_path(cr);
		cairo_arc(cr, radius, radius, radius - radialSectionLength, M_PI + absRedlineArcStartAngle, M_PI + absRedlineArcAngle);
		cairo_rel_line_to(cr, -radialSectionLength * cos(absRedlineArcAngle),
			-radialSectionLength * sin(absRedlineArcAngle));
		cairo_arc_negative(cr, radius, radius, radius, M_PI + absRedlineArcAngle, M_PI + absRedlineArcStartAngle);

		cairo_close_path(cr);
		cairo_fill(cr);
	}
}
