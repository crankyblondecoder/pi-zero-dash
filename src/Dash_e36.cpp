#include "Dash_e36.hpp"

using namespace piZeroDash;

Dash_e36::~Dash_e36()
{
	if(_speedo) delete _speedo;
	if(_tacho) delete _tacho;
	if(_turnLeft) delete _turnLeft;
	if(_turnRight) delete _turnRight;
	if(_engineTemp) delete _engineTemp;
	if(_boost) delete _boost;
	if(_fuelLevel) delete _fuelLevel;
	if(_parkBrake) delete _parkBrake;
	if(_doorOpen) delete _doorOpen;
	if(_lowVoltage) delete _lowVoltage;
	if(_lowOilPressure) delete _lowOilPressure;
	if(_ecuWarning) delete _ecuWarning;
	if(_oilTemp) delete _oilTemp;
}

Dash_e36::Dash_e36(double speedoWidthPercent, double tachoWidthPercent, double turnIndicatorWidthPercent,
	double lightHeightPercent, double engineTempWidthPercent, double boostWidthPercent)
	: _speedo{0}, _tacho{0}, _turnLeft{0}, _turnRight{0}, _headlight{0}, _engineTemp{0}, _boost{0}
{
	Visual::setRootClearColour(0.0, 0.0, 0.0);

	unsigned dashWidth = _getWidth();
	unsigned dashHeight = _getHeight();

	// Speedo.
	// -------

	unsigned speedoWidth = dashWidth * (speedoWidthPercent / 100.0);
	unsigned speedoHeight = (speedoWidth / 2.0) * 1.2;

	unsigned speedoPosnX = ((dashWidth - speedoWidth) / 2);
	unsigned speedoPosnY = dashHeight - speedoHeight;

	_speedo = new GaugeSpeedo_e36(260, speedoPosnX, speedoPosnY, speedoWidth, speedoHeight);

	_addGauge(_speedo);

	struct bounds speedoPrecBoxBounds =  _speedo -> getPreciseSpeedBoxBounds();

	// Tacho.
	// ------

	unsigned tachoWidth = dashWidth * (tachoWidthPercent / 100.0);
	unsigned tachoHeight = tachoWidth / 2;

	unsigned tachoDialCentreX = speedoPosnX + speedoWidth / 2;
	unsigned tachoDialCentreY = speedoPosnY + speedoWidth / 2;

	_tacho = new GaugeTacho_e36(8000, 7500, 7000, true, tachoDialCentreX - (tachoWidth / 2), tachoDialCentreY - tachoHeight,
		tachoWidth, tachoHeight * 1.1);

	_addGauge(_tacho);

	// Turn indicators.
	// ----------------

	unsigned turnWidth = dashWidth * (turnIndicatorWidthPercent / 100.0);

	_turnLeft = new GaugeTurnIndicator_e36(true, 5, 5, turnWidth, turnWidth);
	_turnRight = new GaugeTurnIndicator_e36(false, dashWidth - turnWidth - 5, 5, turnWidth, turnWidth);

	_addGauge(_turnLeft);
	_addGauge(_turnRight);

	// Headlight gauge.
	// ----------------

	unsigned lightHeight = dashHeight * (lightHeightPercent / 100.0);
	unsigned lightWidth = lightHeight;
	unsigned headlightPosnX = (dashWidth - lightWidth) / 2.0;

	_headlight = new GaugeHeadlight_e36(headlightPosnX, 0.0, lightWidth, lightHeight);

	_addGauge(_headlight);

	// Engine temperature gauge.
	// -------------------------

	unsigned engineTempWidth = dashWidth * (engineTempWidthPercent / 100.0);
	unsigned engineTempHeight = (double) engineTempWidth * 0.5;

	double engTempBoxLeft = ((speedoPrecBoxBounds.left + speedoPrecBoxBounds.right) / 2.0) - (engineTempWidth / 2.0);
	double engTempBoxTop = speedoPrecBoxBounds.top - engineTempHeight * 1.1;

	_engineTemp = new GaugeEngineTemp_e36(-10, 110, 50, 96, engTempBoxLeft, engTempBoxTop, engineTempWidth, engineTempHeight);

	_addGauge(_engineTemp);

	// Boost gauge.
	// ------------

	double _boostGaugeWidth = dashWidth * (boostWidthPercent / 100.0);

	_boost = new GaugeBoost_e36(0, 30, 0, dashWidth - _boostGaugeWidth * 1.1, dashHeight - speedoHeight,
		_boostGaugeWidth, _boostGaugeWidth);

	_addGauge(_boost);

	// Fuel level gauge.
	// -----------------

	// For the sake of symmetry, use the same size as the boost gauge.

	_fuelLevel = new GaugeFuelLevel_e36(60, _boostGaugeWidth * 0.1, dashHeight - speedoHeight, _boostGaugeWidth,
		_boostGaugeWidth);

	_addGauge(_fuelLevel);

	// Park brake gauge.
	// -----------------

	double lightSeparationDist = lightWidth * 0.1;

	// Sits to the right of the headlight gauge.
	_parkBrake = new GaugeParkBrake_e36(headlightPosnX + lightWidth + lightSeparationDist, 0.0, lightWidth, lightHeight);

	_addGauge(_parkBrake);

	// Door open gauge.
	// ----------------

	// Sits to the left of the headlight gauge.
	_doorOpen = new GaugeDoorOpen_e36(headlightPosnX - lightWidth - lightSeparationDist, 0.0, lightWidth, lightHeight);

	_addGauge(_doorOpen);

	// Low voltage gauge.
	// ------------------

	_lowVoltage = new GaugeLowVoltage_e36(headlightPosnX - lightWidth * 2.0 - lightSeparationDist * 2.0, 0.0, lightWidth,
		lightHeight);

	_addGauge(_lowVoltage);

	// Low oil pressure gauge.
	// -----------------------

	_lowOilPressure = new GaugeLowOilPressure_e36(headlightPosnX + lightWidth * 2.0 + lightSeparationDist * 2.0, 0.0,
		lightWidth, lightHeight);

	_addGauge(_lowOilPressure);

	// ECU Warning gauge.
	// ------------------

	_ecuWarning = new GaugeEcuWarning_e36(headlightPosnX - lightWidth * 3.0 - lightSeparationDist * 3.0, 0.0, lightWidth,
		lightHeight);

	_addGauge(_ecuWarning);

	// Engine oil temperature gauge.
	// -----------------------------

	_oilTemp = new GaugeOilTemperature_e36(headlightPosnX + lightWidth * 3.0 + lightSeparationDist * 3.0, 0.0, lightWidth,
		lightHeight);

	_addGauge(_oilTemp);
}

void Dash_e36::_drawBackground(CairoSurface& surface)
{
	surface.clear(0.0, 0.0, 0.0, 1.0);
}

void Dash_e36::_drawForeground(CairoSurface& surface)
{
}

void Dash_e36::setPicoLatcher(LatcherPico* picoLatcher)
{
	_speedo -> setInstrumentLatcher(picoLatcher);

	_tacho -> setInstrumentLatcher(picoLatcher);

	_turnLeft -> setInstrumentLatcher(picoLatcher);

	_turnRight -> setInstrumentLatcher(picoLatcher);

	_headlight -> setInstrumentLatcher(picoLatcher);

	_engineTemp -> setInstrumentLatcher(picoLatcher);

	_boost -> setInstrumentLatcher(picoLatcher);

	_fuelLevel -> setInstrumentLatcher(picoLatcher);

	_parkBrake -> setInstrumentLatcher(picoLatcher);

	_doorOpen -> setInstrumentLatcher(picoLatcher);

	_lowVoltage -> setInstrumentLatcher(picoLatcher);

	_lowOilPressure -> setInstrumentLatcher(picoLatcher);

	_oilTemp -> setInstrumentLatcher(picoLatcher);
}

void Dash_e36::setPiZeroLatcher(Latcher* latcher)
{
	// TODO ... Canbus direct to Pi Zero.
	_ecuWarning -> setInstrumentLatcher(latcher);
}
