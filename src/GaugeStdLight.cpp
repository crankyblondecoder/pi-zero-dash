#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeStdLight.hpp"

using namespace piZeroDash;

GaugeStdLight::~GaugeStdLight()
{
}

GaugeStdLight::GaugeStdLight(double cornerRadius, int globalPositionX, int globalPositionY, unsigned width,
	unsigned height)
	: Gauge(globalPositionX, globalPositionY, width, height)
{
	_cornerRadius = cornerRadius;
}

void GaugeStdLight::_drawDefaultPath(cairo_t* cr)
{
	double width = _getWidth();
	double height = _getHeight();

	cairo_identity_matrix(cr);

	_drawDefaultBoxPath(cr, _cornerRadius, 0.0, width, 0.0, height);
}
