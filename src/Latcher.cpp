#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "Latcher.hpp"

using namespace piZeroDash;

Latcher* Latcher::__currentLatcher;

void Latcher::setCurrentLatcher(Latcher* latcher)
{
	__currentLatcher = latcher;
}

// -- Non class thread entry --
void* _threadEntry(void* thread)
{
    ((Latcher*)thread) -> __threadEntry();

    return NULL;
}

Latcher::~Latcher()
{
	// It is assumed latchers are not created/destroyed rapidly but exist for the entire program duration.
	// Hence no protection for the following.

	if(!_stop)
	{
		_stop = true;

		// Very basic and course grained sleep to wait on thread stop.
		sleep(1);

		if(_polling) {

			// Thread still active. Just kill it.
			pthread_cancel(_workThread);
		}
	}
}

Latcher::Latcher()
{
}

bool Latcher::start(long pollingInterval)
{
	_pollingInterval = pollingInterval;

	// Start the polling thread.

	int error = pthread_create(&_workThread, NULL, &_threadEntry, (void*) this);

	return error == 0;
}

void Latcher::stop()
{
	_stop = true;
}

void Latcher::__threadEntry()
{
	_polling = true;

	while(!_stop)
	{
		struct timeval curTime;

		gettimeofday(&curTime, 0);

		long nanoSeconds = (curTime.tv_sec - _lastPollSec) * 1000000 + (curTime.tv_usec - _lastPollUSec);

		long pollTimeDiff = _pollingInterval - nanoSeconds;

		if(pollTimeDiff > 0)
		{
			// Need to wait for next polling window.
			timespec sleepDuration;
			sleepDuration.tv_sec = 0;
			sleepDuration.tv_nsec = pollTimeDiff;

			// Ignore any error.
			nanosleep(&sleepDuration, NULL);
		}

		_poll();

		// Save end of poll time.
		gettimeofday(&curTime, 0);
		_lastPollSec = curTime.tv_sec;
		_lastPollUSec = curTime.tv_usec;
	}

	_polling = false;
}