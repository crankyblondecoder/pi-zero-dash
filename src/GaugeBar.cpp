#include <cmath>
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;

#include "GaugeBar.hpp"

using namespace piZeroDash;

GaugeBar::~GaugeBar()
{
}

GaugeBar::GaugeBar(int globalPositionX, int globalPositionY, unsigned width, unsigned height)
	: Gauge(globalPositionX, globalPositionY, width, height)
{
}

void GaugeBar::_drawDefaultBackground(CairoSurface& surface)
{

}

void GaugeBar::_drawDefaultForeground(CairoSurface& surface)
{

}
