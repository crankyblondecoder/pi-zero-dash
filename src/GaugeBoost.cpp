#include <cmath>
#include <iostream>
#include <string>
#include <sys/time.h>

using namespace std;

#include "GaugeBoost.hpp"

using namespace piZeroDash;

GaugeBoost::~GaugeBoost()
{
}

GaugeBoost::GaugeBoost(int minBoost, int maxBoost, int neutralBoost, double radius, double dialCentreX,
	double dialCentreY, int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: GaugeDial(radius, dialCentreX, dialCentreY, globalPositionX, globalPositionY, width, height)
{
	_minBoost = minBoost;
	_maxBoost = maxBoost;
	_neutralBoost = neutralBoost;

	_addInstrument(&_boostInstr);
}

void GaugeBoost::test()
{
	// Go slightly over max so that this likely edge case is tested.
	_boostInstr.test(_minBoost - 5, _maxBoost + 5);
}

bool GaugeBoost::inTestMode()
{
	return _boostInstr.inTestMode();
}

void GaugeBoost::_setStandardProperties(double markedFontSize, colour& markedFontColour, unsigned markedFontDecimalPlaces,
	double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset, colour& majorLineColour,
	colour& minorLineColour, colour& preNeutralColour, colour& postNeutralColour, bool useIndicatorSections,
	double startAngle, double endAngle)
{
	GaugeDial::_setStandardProperties(_minBoost, _maxBoost, 5, true, true, true, markedFontSize,
		markedFontColour, 0, lineLength, majorLineWidth, minorLineWidth, lineStartOffset, majorLineColour, minorLineColour,
		startAngle, endAngle);

	_useIndicatorSections = useIndicatorSections;

	// Generate standard radial sections for section mode.

	// Below neutral boost.
	_standardRadialSections[0].flash = false;
	_standardRadialSections[0].sectionColour = preNeutralColour;
	_standardRadialSections[0].indicatedValueStart = _minBoost - 5;
	_standardRadialSections[0].indicatedValueEnd = _neutralBoost;
	_standardRadialSections[0].onlyShowIfWithinRange = false;

	// Above neutral boost.
	_standardRadialSections[1].flash = false;
	_standardRadialSections[1].sectionColour = postNeutralColour;
	_standardRadialSections[1].indicatedValueStart = _neutralBoost;
	_standardRadialSections[1].indicatedValueEnd = _maxBoost + 5;
	_standardRadialSections[1].onlyShowIfWithinRange = false;
}

void GaugeBoost::_drawDefaultForeground(CairoSurface& surface, double sectionRadialLength, double indicatorLineLength,
	double indicatorLineWidth, colour& indicatorLineColour)
{
	double curBoost = _boostInstr.getBoost();

	if(_useIndicatorSections)
	{
		_drawStandardIndicatorSections(surface, curBoost, sectionRadialLength, _standardRadialSections, 2, true);
	}
	else
	{
		_drawStandardIndicatorLine(surface, curBoost, indicatorLineLength, indicatorLineWidth, indicatorLineColour);
	}
}
