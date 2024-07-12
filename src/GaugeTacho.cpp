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
	_setStandardProperties(0, (double) _maxRpm / 1000.0, 1, true, false, false, markedRpmFontSize,
		markedRpmFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour,
		M_PI, 2.0 * M_PI);

	// Generate standard radial sections

	// Normal
	_standardRadialSections[0].flash = false;
	_standardRadialSections[0].sectionColour = normalColour;
	_standardRadialSections[0].indicatedValueStart = 0.0;
	_standardRadialSections[0].indicatedValueEnd = (double) _redlineWarningRpm / 1000.0;
	_standardRadialSections[0].onlyShowIfWithinRange = false;

	// Redline warning.
	_standardRadialSections[1].flash = false;
	_standardRadialSections[1].sectionColour = redlineWarningThresholdColour;
	_standardRadialSections[1].indicatedValueStart = (double) _redlineWarningRpm / 1000.0;
	_standardRadialSections[1].indicatedValueEnd = (double) _redlineRpm / 1000.0;
	_standardRadialSections[1].onlyShowIfWithinRange = false;

	// Redline.
	_standardRadialSections[2].flash = _flashRedline;
	_standardRadialSections[2].sectionColour = redlineColour;
	_standardRadialSections[2].indicatedValueStart = (double) _redlineRpm / 1000.0;
	_standardRadialSections[2].indicatedValueEnd = (double) _maxRpm / 1000.0;
	_standardRadialSections[2].onlyShowIfWithinRange = false;
}

void GaugeTacho::_drawDefaultForeground(CairoSurface& surface, double sectionRadialLength)
{
	double curRpm = _tachoInstr.getRpm();

	_drawStandardIndicatorSections(surface, curRpm / 1000.0, sectionRadialLength, _standardRadialSections, 3, true);
}
