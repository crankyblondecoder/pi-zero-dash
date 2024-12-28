#include <iostream>
#include <signal.h>
#include <stdlib.h>

using namespace std;

#include "Dash_e36.hpp"
#include "Latcher.hpp"
#include "LatcherPico.hpp"

using namespace piZeroDash;

Dash_e36* e36Dash;

void handleSigInt(int sigNum)
{
	cout << "Exiting triggered from SIGINT.\n";

	e36Dash -> stopStrobing();
}

int main(int argc, char* argv[])
{
	signal(SIGINT, handleSigInt);

	cout << "*** Pi Zero Dash ***\n\n";

	// The dri card number.
	int cardNumber = 0;

	// The dri connector index on the card.
	int connectorIndex = 0;

	// The mode index on the connector to use.
	int modeIndex = 0;

	if(argc > 1)
	{
		// The second param is the card number.
		cardNumber = atoi(argv[2]);
	}

	if(argc > 2)
	{
		// The third param is the connector index.
		connectorIndex = atoi(argv[3]);
	}

	if(argc > 3)
	{
		// The fourth param is the mode index.
		modeIndex = atoi(argv[4]);
	}

	cout << "Attempting to bind DRM device with CardNumber:" << cardNumber << "  ConnectorIndex:" << connectorIndex << "  ModeIndex:  "
	     << modeIndex << "\n\n";

	// Use an adapsurf drm device.
	bool bound = Visual::bindToDrmDevice(cardNumber, connectorIndex, modeIndex);

	if(!bound)
	{
		cout << "Could not bind to drm device. Exiting.\n";
		exit(1);
	}

	cout << "Bound to DRM device okay.\n";

	LatcherPico picoLatcher;

	// Latch data from the Pico 100 times per second.
	picoLatcher.start(10000);

	// For now just default to using the E36 dash. This can be easily configurable on the command line.
	e36Dash = new Dash_e36(78, 50, 8, 10, 12, 30);
	e36Dash -> setPicoLatcher(&picoLatcher);
	e36Dash -> strobe(0);

	delete e36Dash;

	picoLatcher.stop();

	cout << "Exiting Pi Zero Dash. Have a nice day.\n\n";

	return 0;
}
