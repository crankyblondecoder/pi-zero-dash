#include <cstring>
#include <iostream>

#include "adapsurf/DrmDevice.hpp"
#include "adapsurf/Exception.hpp"

#include "Visual.hpp"

using namespace piZeroDash;

// Global adapsurf device.
DrmDevice* Visual::adsDevice = 0;

Visual::~Visual()
{
	if(_backgroundSurface) delete _backgroundSurface;

	if(_foregroundSurface) delete _foregroundSurface;
}

Visual::Visual(int globalPositionX, int globalPositionY, unsigned width, unsigned height, bool hasForeground,
	bool hasBackground) : _backgroundSurface{0}, _foregroundSurface{0}
{
	// At this stage, visuals are only positioned globally.

	if(hasBackground)
	{
		_backgroundSurface = new CairoSurface(globalPositionX, globalPositionY, width, height);
	}

	if(hasForeground)
	{
		_foregroundSurface = new CairoSurface(globalPositionX, globalPositionY, width, height);
	}

	_width = width;
	_height = height;
}

bool Visual::bindToDrmDevice(unsigned cardNumber, int connectorIndex, int modeIndex)
{
	bool success = true;

	try
	{
		adsDevice = new adapsurf::DrmDevice(cardNumber, connectorIndex, modeIndex);
	}
	catch(const adapsurf::Exception& ex)
	{
		std::cout << ex.getErrorDescr() << "\n";
		success = false;
	}

	return success;
}

void Visual::setRootClearColour(double red, double green, double blue)
{
	if(adsDevice)
	{
		adsDevice -> setClearColour(red, green, blue);
	}
	else
	{
		std::cout << "Could not set clear colour because adsDevice has not been set.\n";
	}
}

void Visual::drawBackground()
{
	if(_backgroundSurface) _drawBackground(*_backgroundSurface);
}

void Visual::_drawBackground()
{
	if(_backgroundSurface) _drawBackground(*_backgroundSurface);
}

void Visual::drawForeground()
{
	if(_foregroundSurface) _drawForeground(*_foregroundSurface);
}

void Visual::_drawForeground()
{
	if(_foregroundSurface) _drawForeground(*_foregroundSurface);
}

void Visual::_composeBackground(Visual& visual)
{
	if(_backgroundSurface && visual._backgroundSurface)
	{
		_backgroundSurface -> compose(*visual._backgroundSurface);
	}
}

void Visual::_composeForeground(Visual& visual)
{
	if(_foregroundSurface && visual._foregroundSurface)
	{
		_foregroundSurface -> compose(*visual._foregroundSurface);
	}
}

void Visual::_composeBackgroundToDisplay()
{
	if(Visual::adsDevice)
	{
		Framebuffer* fb = Visual::adsDevice -> getDrawToFramebuffer();

		if(fb)
		{
			if(_backgroundSurface)
			{
				fb -> compose(*_backgroundSurface);
			}
		}
	}
}

void Visual::_composeForegroundToDisplay()
{
	if(Visual::adsDevice)
	{
		Framebuffer* fb = Visual::adsDevice -> getDrawToFramebuffer();

		if(fb)
		{
			if(_foregroundSurface)
			{
				fb -> compose(*_foregroundSurface);
			}
		}
	}
}

void Visual::_commitToDisplay()
{
	// This should block until page flip has completed.
	Visual::adsDevice -> pageFlip();
}

unsigned Visual::_getWidth()
{
	return _width;
}

unsigned Visual::_getHeight()
{
	return _height;
}