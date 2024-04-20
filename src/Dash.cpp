#include "Dash.hpp"
#include "Exception.hpp"

using namespace piZeroDash;

Dash::Dash() : Visual(0, 0, (unsigned) Visual::adsDevice -> getDisplayResolutionWidth(),
	(unsigned) Visual::adsDevice -> getDisplayResolutionHeight(), false, true)
{
	_gauges = new Gauge*[NUM_GAUGES];
	_gaugeCount = 0;
}

void Dash::_addGauge(Gauge* gauge)
{
	if(_gaugeCount >= NUM_GAUGES)
	{
		std::string msg("Maximum number of gauges exceeded.");
		throw Exception(Exception::Error::TOO_MANY_GAUGES, msg);
	}
}

void Dash::__generatedBackground()
{
	TODO;
}