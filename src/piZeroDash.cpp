#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Latcher.hpp"
#include "LatcherPico.hpp"

using namespace piZeroDash;

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		// The first param.
		//atoi(argv[1]);
	}

	cout << "*** Pi Zero Dash ***";

	LatcherPico curLatcher;

	Latcher::setCurrentLatcher(&curLatcher);

	// TODO ... Thready stuff.

	Latcher::setCurrentLatcher(0);
}
