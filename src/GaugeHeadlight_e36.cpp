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
	bool lowBeamState = _headLightLowBeamInstr.getOnOffState();

	bool highBeamState;

	if(_headLightLowBeamInstr.inTestMode())
	{
		_testHighBeamActive = !_testHighBeamActive && _testLowBeamActive;
		_testLowBeamActive =

		lowBeamState = _testLowBeamActive;
		highBeamState = _testHighBeamActive;
	}
	else
	{
		highBeamState = _headLightHighBeamInstr.getOnOffState();
	}


}

void GaugeHeadlight_e36::test()
{
	// Just use the low beam intrument for all tests.
	_headLightLowBeamInstr.test();
}

bool GaugeHeadlight_e36::inTestMode()
{
	return _headLightHighBeamInstr.inTestMode();
}