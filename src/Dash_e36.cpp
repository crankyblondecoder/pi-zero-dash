#include "Dash_e36.hpp"

using namespace piZeroDash;

Dash_e36::~Dash_e36()
{
	if(_speedo) delete _speedo;
}

Dash_e36::Dash_e36(double speedoWidthPercent) : _speedo{0}
{
	Visual::setRootClearColour(0.0, 0.0, 0.0);

	unsigned dashWidth = _getWidth();
	unsigned dashHeight = _getHeight();

	unsigned speedoWidth = dashWidth * (speedoWidthPercent / 100.0);
	unsigned speedoHeight = (speedoWidth / 2.0) * 1.2;

	_speedo = new GaugeSpeedo_e36(260, 0.05 * (double) speedoWidth, 0.05 * (double) speedoWidth, 0.01 * (double) speedoWidth,
		0.005 * (double) speedoWidth, ((dashWidth - speedoWidth) / 2), dashHeight - speedoHeight, speedoWidth, speedoHeight);

	_addGauge(_speedo);
}

void Dash_e36::_drawBackground(CairoSurface& surface)
{
	surface.clear(0.0, 0.0, 0.0, 1.0);
}

void Dash_e36::_drawForeground(CairoSurface& surface)
{
}
