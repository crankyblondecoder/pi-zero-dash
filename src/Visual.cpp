#include <cstring>
#include <iostream>

#include "adapsurf/DrmDevice.hpp"
#include "adapsurf/Exception.hpp"

#include "Visual.hpp"

using namespace piZeroDash;

// Global adapsurf device.
adapsurf::Device* Visual::adsDevice;

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