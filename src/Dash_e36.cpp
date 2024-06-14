#include "Dash_e36.hpp"

using namespace piZeroDash;

Dash_e36::~Dash_e36()
{
	if(_speedo) delete _speedo;
	if(_tacho) delete _tacho;
}

Dash_e36::Dash_e36(double speedoWidthPercent, double tachoWidthPercent) : _speedo{0}
{
	Visual::setRootClearColour(0.0, 0.0, 0.0);

	unsigned dashWidth = _getWidth();
	unsigned dashHeight = _getHeight();

	unsigned speedoWidth = dashWidth * (speedoWidthPercent / 100.0);
	unsigned speedoHeight = (speedoWidth / 2.0) * 1.2;

	unsigned speedoPosnX = ((dashWidth - speedoWidth) / 2);
	unsigned speedoPosnY = dashHeight - speedoHeight;

	_speedo = new GaugeSpeedo_e36(260, speedoPosnX, speedoPosnY, speedoWidth, speedoHeight);

	_addGauge(_speedo);

	unsigned tachoWidth = dashWidth * (tachoWidthPercent / 100.0);
	unsigned tachoHeight = tachoWidth / 2;

	unsigned dialCentreX = speedoPosnX + speedoWidth / 2;
	unsigned dialCentreY = speedoPosnY + speedoWidth / 2;

	_tacho = new GaugeTacho_e36(8000, dialCentreX - (tachoWidth / 2), dialCentreY - tachoHeight, tachoWidth, tachoHeight);

	_addGauge(_tacho);
}

void Dash_e36::_drawBackground(CairoSurface& surface)
{
	surface.clear(0.0, 0.0, 0.0, 1.0);
}

void Dash_e36::_drawForeground(CairoSurface& surface)
{
}
