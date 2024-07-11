#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Dash_e36.hpp"
#include "Visual.hpp"

using namespace piZeroDash;

// Verbosity level.
int verbosity = 1;

int main(int argc, char* argv[])
{
	// The dri card number.
	int cardNumber = 0;

	// The dri connector index on the card.
	int connectorIndex = 0;

	// The mode index on the connector to use.
	int modeIndex = 0;

	if(argc > 1)
	{
		// The first param is the verbosity number.
		verbosity = atoi(argv[1]);
	}

	if(argc > 2)
	{
		// The second param is the card number.
		cardNumber = atoi(argv[2]);
	}

	if(argc > 3)
	{
		// The third param is the connector index.
		connectorIndex = atoi(argv[3]);
	}

	if(argc > 4)
	{
		// The fourth param is the mode index.
		modeIndex = atoi(argv[4]);
	}

	cout << "*** Pi Zero Dash Test ***\n";

	// Use a adapsurf drm device.
	bool bound = Visual::bindToDrmDevice(cardNumber, connectorIndex, modeIndex);

	if(!bound)
	{
		cout << "Could not bind to drm device. Test failed.\n";
	}

	// Generate and put into a test cycle an E36 dash.
	Dash_e36 e36Dash(78, 50, 8, 10, 12, 30);
	e36Dash.test();

	// Wait for 'q' then enter key.
	cout << "Press any key, then enter.";
	string scratch;
	cin >> scratch;
}
