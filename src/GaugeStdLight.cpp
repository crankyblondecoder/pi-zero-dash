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

	double left = 0.0;
	double right = width;
	double top = 0.0;
	double bottom = height;

	cairo_new_sub_path(cr);
	cairo_arc(cr, left + _cornerRadius, top + _cornerRadius, _cornerRadius, M_PI, 1.5 * M_PI);
	cairo_line_to(cr, right - _cornerRadius, top);
	cairo_arc(cr, right - _cornerRadius, top + _cornerRadius, _cornerRadius, 1.5 * M_PI, 2.0 * M_PI);
	cairo_line_to(cr, right, bottom - _cornerRadius);
	cairo_arc(cr, right - _cornerRadius, bottom - _cornerRadius, _cornerRadius, 0.0, 0.5 * M_PI);
	cairo_line_to(cr, left - _cornerRadius, bottom);
	cairo_arc(cr, left + _cornerRadius, bottom - _cornerRadius, _cornerRadius, 0.5 * M_PI, M_PI);
	cairo_close_path(cr);
}
