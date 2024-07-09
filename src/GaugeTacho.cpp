#include <cmath>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

#include "GaugeTacho.hpp"

using namespace piZeroDash;

GaugeTacho::~GaugeTacho()
{
}

GaugeTacho::GaugeTacho(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, bool flashRedline, double radius,
	double dialCentreX, double dialCentreY, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeDial(radius, dialCentreX, dialCentreY, globalPositionX, globalPositionY, width, height)
{
	_maxRpm = maxRpm;
	_redlineRpm = redlineRpm;
	_redlineWarningRpm = redlineWarningRpm;
	_flashRedline = flashRedline;

	_addInstrument(&_tachoInstr);
}

unsigned GaugeTacho::_getMaxRpm()
{
	return _maxRpm;
}

void GaugeTacho::test()
{
	// Go slightly over max so that this likely edge case is tested.
	_tachoInstr.test(_maxRpm + 200);
}

bool GaugeTacho::inTestMode()
{
	return _tachoInstr.inTestMode();
}

void GaugeTacho::_setProperties(double markedRpmFontSize, colour& markedRpmFontColour, unsigned markedRpmFontDecimalPlaces,
	double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset, colour& majorLineColour,
	colour& minorLineColour, colour& normalColour, colour& redlineWarningThresholdColour, colour& redlineColour)
{
	_setStandardProperties(0, (double) _maxRpm / 1000, 1, true, true, true, markedRpmFontSize,
		markedRpmFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour,
		M_PI, 2.0 * M_PI);

	// Generate standard radial sections

	// Normal
	_standardRadialSections[0].flash = false;
	_standardRadialSections[0].sectionColour = normalColour;
	_standardRadialSections[0].indicatedValueStart = 0.0;
	_standardRadialSections[0].indicatedValueEnd = _redlineWarningRpm;

	// Redline warning.
	_standardRadialSections[1].flash = false;
	_standardRadialSections[1].sectionColour = redlineWarningThresholdColour;
	_standardRadialSections[1].indicatedValueStart = _redlineWarningRpm;
	_standardRadialSections[1].indicatedValueEnd = _redlineRpm;

	// Redline.
	_standardRadialSections[2].flash = _flashRedline;
	_standardRadialSections[2].sectionColour = redlineColour;
	_standardRadialSections[2].indicatedValueStart = _redlineRpm;
	_standardRadialSections[2].indicatedValueEnd = _maxRpm;
}

void GaugeTacho::_drawDefaultForeground(CairoSurface& surface, double sectionRadialLength, colour& normalColour,
	colour& redlineWarningThresholdColour, colour& redlineColour)
{
	bool draw = true;

	unsigned curRpm = _tachoInstr.getRpm();

	if(_flashRedline)
	{
		if(curRpm > _redlineRpm)
		{
			struct timeval curTime;

			gettimeofday(&curTime, 0);

			long millis = (curTime.tv_sec - _redlineLastShowSec) * 1000 + (curTime.tv_usec - _redlineLastShowUSec) / 1000;

			if(!_redlineSectionActive || millis > _redlineSectionFlashPeriod * 2)
			{
				_redlineLastShowSec = curTime.tv_sec;
				_redlineLastShowUSec = curTime.tv_usec;
			}

			draw = !_redlineSectionActive || millis < _redlineSectionFlashPeriod;

			_redlineSectionActive = true;
		}
		else
		{
			_redlineSectionActive = false;
		}
	}

	// Flashing is active.
	if(!draw) return;

	cairo_t* cr = surface.getContext();

	double radius = _getRadius();
	double dialCentreX = _getDialCentreX();
	double dialCentreY = _getDialCentreY();

	double degreesPerRpm = (M_PI / (double) _maxRpm);

	// Draw normal rpm range.

	unsigned normalEnd = curRpm > _redlineWarningRpm ? _redlineWarningRpm : curRpm;

	double normalArcAngle = degreesPerRpm * (double) normalEnd;

	cairo_identity_matrix(cr);

	cairo_set_source_rgba(cr, normalColour.r, normalColour.g, normalColour.b, normalColour.a);
	cairo_move_to(cr, dialCentreX - radius, dialCentreY);
	cairo_line_to(cr, dialCentreX - radius + sectionRadialLength, dialCentreY);
	cairo_arc(cr, dialCentreX, dialCentreY, radius - sectionRadialLength, M_PI, M_PI + normalArcAngle);
	cairo_rel_line_to(cr, -sectionRadialLength * cos(normalArcAngle), -sectionRadialLength * sin(normalArcAngle));
	cairo_arc_negative(cr, dialCentreX, dialCentreY, radius, M_PI + normalArcAngle, M_PI);

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
		cairo_arc(cr, dialCentreX, dialCentreY, radius - sectionRadialLength, M_PI + normalArcAngle, M_PI + absRedlineWarningArcAngle);
		cairo_rel_line_to(cr, -sectionRadialLength * cos(absRedlineWarningArcAngle),
			-sectionRadialLength * sin(absRedlineWarningArcAngle));
		cairo_arc_negative(cr, dialCentreX, dialCentreY, radius, M_PI + absRedlineWarningArcAngle, M_PI + normalArcAngle);

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
		cairo_arc(cr, dialCentreX, dialCentreY, radius - sectionRadialLength, M_PI + absRedlineArcStartAngle, M_PI + absRedlineArcAngle);
		cairo_rel_line_to(cr, -sectionRadialLength * cos(absRedlineArcAngle),
			-sectionRadialLength * sin(absRedlineArcAngle));
		cairo_arc_negative(cr, dialCentreX, dialCentreY, radius, M_PI + absRedlineArcAngle, M_PI + absRedlineArcStartAngle);

		cairo_close_path(cr);
		cairo_fill(cr);
	}
}
