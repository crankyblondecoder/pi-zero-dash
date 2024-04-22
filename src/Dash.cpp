#include "Dash.hpp"
#include "Exception.hpp"

using namespace piZeroDash;

Dash::Dash() : Visual(0, 0, (unsigned) Visual::adsDevice -> getDisplayResolutionWidth(),
	(unsigned) Visual::adsDevice -> getDisplayResolutionHeight(), false, true)
{
	_gauges = new Gauge*[NUM_GAUGES];
	_gaugeCount = 0;
	_strobing = false;
}

void Dash::_addGauge(Gauge* gauge)
{
	if(_gaugeCount >= NUM_GAUGES)
	{
		std::string msg("Maximum number of gauges exceeded.");
		throw Exception(Exception::Error::TOO_MANY_GAUGES, msg);
	}

	_gauges[_gaugeCount++] = gauge;
}

void Dash::__generateBackgrounds()
{
	// Draw background of this dash.
	_drawBackground();

	// Draw background of all gauges and compose them onto this dashes background.
	for(unsigned index = 0; index < _gaugeCount; index++)
	{
		_gauges[index] -> drawBackground();

		_composeBackground(*_gauges[index]);
	}

	_backgroundsGenerated = true;
}

void Dash::__scan()
{
	TODO;
}

void Dash::strobe(unsigned numberOfStrobes)
{
	if(!_backgroundsGenerated) __generateBackgrounds();

	bool continuous = numberOfStrobes == 0;

	_strobing = true;

	while(_strobing && (continuous || numberOfStrobes > 0))
	{
		// Clear the foreground so that the previous gauge scans don't leave stuff behind.
		TODO;

		// Scan all the gauges.
		__scan();

		if(!continuous) numberOfStrobes--;

		// Write the foreground and background to the displays back buffer.
		_composeToDisplay();
	}

	_strobing = false;
}

void Dash::stopStrobing()
{
	_strobing = false;
}