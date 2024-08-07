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
	: GaugeDial(radius, dialCentreX, dialCentreY, globalPositionX, globalPositionY, width, height, false)
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
	colour& minorLineColour, double startAngle, double endAngle, double lowFuelLevel, colour& lowFuelLevelIndicatorColour,
	colour& belowZeroFuelLevelIndicatorColour)
{
	GaugeDial::_setStandardProperties(0, _maxFuelLevel, 7.5, true, false, false, markedFontSize,
		markedFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour,
		startAngle, endAngle);

	_lineLength = lineLength;

	// Below zero fuel light.
	_standardRadialSections[0].flash = false;
	_standardRadialSections[0].sectionColour = belowZeroFuelLevelIndicatorColour;
	_standardRadialSections[0].indicatedValueStart = -3.0;
	_standardRadialSections[0].indicatedValueEnd = 0.0;
	_standardRadialSections[0].onlyShowIfWithinRange = true;

	// Low fuel light.
	_standardRadialSections[1].flash = false;
	_standardRadialSections[1].sectionColour = lowFuelLevelIndicatorColour;
	_standardRadialSections[1].indicatedValueStart = 0.0;
	_standardRadialSections[1].indicatedValueEnd = lowFuelLevel;
	_standardRadialSections[1].onlyShowIfWithinRange = true;
}

void GaugeFuelLevel::_drawDefaultForeground(CairoSurface& surface, double indicatorLineLength,
	double indicatorLineWidth, colour& indicatorLineColour)
{
	_lastFuelLevel = _fuelLevelInstr.getFuelVolume();

	_drawStandardIndicatorSections(surface, _lastFuelLevel, _lineLength, _standardRadialSections, 2, false);

	_drawStandardIndicatorLine(surface, _lastFuelLevel, indicatorLineLength, indicatorLineWidth, indicatorLineColour);
}

bool GaugeFuelLevel::_requiresDrawForeground(Instrument* instrument)
{
	if(instrument == &_fuelLevelInstr)
	{
		return floor(_lastFuelLevel * 2.0) / 2.0 != floor(_fuelLevelInstr.getFuelVolume() * 2.0) / 2.0;
	}

	return false;
}