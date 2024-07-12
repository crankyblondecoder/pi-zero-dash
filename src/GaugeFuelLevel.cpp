#include <cmath>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

#include "GaugeFuelLevel.hpp"

using namespace piZeroDash;

GaugeFuelLevel::~GaugeFuelLevel()
{
}

GaugeFuelLevel::GaugeFuelLevel(int maxFuelLevel, double radius, double dialCentreX,
	double dialCentreY, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeDial(radius, dialCentreX, dialCentreY, globalPositionX, globalPositionY, width, height)
{
	_maxFuelLevel = maxFuelLevel;

	_addInstrument(&_fuelLevelInstr);
}

void GaugeFuelLevel::test()
{
	// Go slightly over max so that this likely edge case is tested.
	_fuelLevelInstr.test(_maxFuelLevel);
}

bool GaugeFuelLevel::inTestMode()
{
	return _fuelLevelInstr.inTestMode();
}

void GaugeFuelLevel::_setStandardProperties(double markedFontSize, colour& markedFontColour, unsigned markedFontDecimalPlaces,
	double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset, colour& majorLineColour,
	colour& minorLineColour, double startAngle, double endAngle, double lowFuelLevel, colour& lowFuelLevelIndicatorColour)
{
	GaugeDial::_setStandardProperties(0, _maxFuelLevel, 7.5, true, false, false, markedFontSize,
		markedFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour,
		startAngle, endAngle);

	_lineLength = lineLength;

	// Low fuel light.
	_standardRadialSections[0].flash = false;
	_standardRadialSections[0].sectionColour = lowFuelLevelIndicatorColour;
	_standardRadialSections[0].indicatedValueStart = -3.0;
	_standardRadialSections[0].indicatedValueEnd = lowFuelLevel;
	_standardRadialSections[0].onlyShowIfWithinRange = true;
}

void GaugeFuelLevel::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength,
	double indicatorLineWidth, colour& indicatorLineColour)
{
	double curFuelLevel = _fuelLevelInstr.getFuelVolume();

	_drawStandardIndicatorSections(surface, curFuelLevel, _lineLength, _standardRadialSections, 1, false);

	_drawStandardIndicatorLine(surface, curFuelLevel, indicatorLineLength, indicatorLineWidth, indicatorLineColour);
}
