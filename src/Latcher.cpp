#include <sys/time.h>
#include <time.h>

#include "Latcher.hpp"

using namespace piZeroDash;

// -- Non class thread entry --
void* _threadEntry(void* thread)
{
    ((Latcher*)thread) -> __threadEntry();

    return NULL;
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
}