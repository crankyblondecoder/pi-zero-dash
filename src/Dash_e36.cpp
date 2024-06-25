#include "Dash_e36.hpp"

using namespace piZeroDash;

Dash_e36::~Dash_e36()
{
	if(_speedo) delete _speedo;
	if(_tacho) delete _tacho;
	if(_turnLeft) delete _turnLeft;
	if(_turnRight) delete _turnRight;
	if(_engineTemp) delete _engineTemp;
}

Dash_e36::Dash_e36(double speedoWidthPercent, double tachoWidthPercent, double turnIndicatorWidthPercent,
	double lightHeightPercent, double engineTempWidthPercent)
	: _speedo{0}, _tacho{0}, _turnLeft{0}, _turnRight{0}, _headlight{0}, _engineTemp{0}
{
	Visual::setRootClearColour(0.0, 0.0, 0.0);

	unsigned dashWidth = _getWidth();
	unsigned dashHeight = _getHeight();

	// Speedo.

	unsigned speedoWidth = dashWidth * (speedoWidthPercent / 100.0);
	unsigned speedoHeight = (speedoWidth / 2.0) * 1.2;

	unsigned speedoPosnX = ((dashWidth - speedoWidth) / 2);
	unsigned speedoPosnY = dashHeight - speedoHeight;

	_speedo = new GaugeSpeedo_e36(260, speedoPosnX, speedoPosnY, speedoWidth, speedoHeight);

	_addGauge(_speedo);

	// Tacho.

	unsigned tachoWidth = dashWidth * (tachoWidthPercent / 100.0);
	unsigned tachoHeight = tachoWidth / 2;

	unsigned tachoDialCentreX = speedoPosnX + speedoWidth / 2;
	unsigned tachoDialCentreY = speedoPosnY + speedoWidth / 2;

	_tacho = new GaugeTacho_e36(8000, 7500, 7000, true, tachoDialCentreX - (tachoWidth / 2), tachoDialCentreY - tachoHeight, tachoWidth,
		tachoHeight * 1.1);

	_addGauge(_tacho);

	// Turn indicators.

	unsigned turnWidth = dashWidth * (turnIndicatorWidthPercent / 100.0);

	_turnLeft = new GaugeTurnIndicator_e36(true, 5, 5, turnWidth, turnWidth);
	_turnRight = new GaugeTurnIndicator_e36(false, dashWidth - turnWidth - 5, 5, turnWidth, turnWidth);

	_addGauge(_turnLeft);
	_addGauge(_turnRight);

	// Headlight gauge.

	unsigned lightHeight = dashHeight * (lightHeightPercent / 100.0);
	unsigned lightWidth = lightHeight;

	_headlight = new GaugeHeadlight_e36((dashWidth - lightWidth) / 2.0, 0.0, lightWidth, lightHeight);

	_addGauge(_headlight);

	// Engine temperature gauge.

	unsigned engineTempWidth = dashWidth * (engineTempWidthPercent / 100.0);
	unsigned engineTempHeight = (double) engineTempWidth * 0.5;

	_engineTemp = new GaugeEngineTemp_e36(-10, 110, 50, 96, (dashWidth - engineTempWidth) / 2.0,
		tachoDialCentreY - (tachoHeight / 2) + 10, engineTempWidth, engineTempHeight);

	_addGauge(_engineTemp);
}

void Dash_e36::_drawBackground(CairoSurface& surface)
{
	surface.clear(0.0, 0.0, 0.0, 1.0);
}

void Dash_e36::_drawForeground(CairoSurface& surface)
{
}
