#include <cmath>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeTurnIndicator_e36.hpp"

using namespace piZeroDash;

GaugeTurnIndicator_e36::~GaugeTurnIndicator_e36()
{
}

GaugeTurnIndicator_e36::GaugeTurnIndicator_e36(bool left, int globalPositionX, int globalPositionY, unsigned width,
	unsigned height)
	: GaugeTurnIndicator(left, globalPositionX, globalPositionY, width, height)
{
}

void GaugeTurnIndicator_e36::_drawBackground(CairoSurface& surface)
{
	_drawDefaultBackground(surface, _backgroundColour);
}

void GaugeTurnIndicator_e36::_drawForeground(CairoSurface& surface)
{
	_drawDefaultForeground(surface, _foregroundColour);
}
