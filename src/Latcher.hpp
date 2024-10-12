#ifndef PZD_LATCHER_H
#define PZD_LATCHER_H

#include <pthread.h>

/** Maximum number of unique latched data values. */
#define MAX_LATCHED_INDEXES 64

namespace piZeroDash
{
	/**
	 * Generic base class for all sources of latched data.
	 */
	class Latcher
	{
		public:

			virtual ~Latcher();
			Latcher();

			/**
			 * Starting polling for latched data.
			 * @param pollingInterval Interval at which latched data is polled. In micro seconds.
			 * @returns True of successful. False otherwise.
			 */
			bool start(long pollingInterval);

			/**
			 * Stop latched data polling.
			 */
			void stop();

			/** Initial thread entry point. */
        	void __threadEntry();

			/**
			 * Used to request a particular latched data value.
			 */
			enum LatchedDataIndex
			{
				/** Engine RPM. */
				ENGINE_RPM,

				/** Speed km/h. */
				SPEED_KMH,

				/** Engine temperature degrees celsius. */
				ENGINE_TEMP_C
			};

			static void setCurrentLatcher(Latcher* latcher);

		protected:

			/** Polling function hook. Called for each polling interval. */
			virtual void _poll() = 0;

			/**
			 * Whether concrete implementation of a latcher is ready to supply latched data.
			 * If a latcher is not ready by the time start is called it will not start.
			 */
			virtual bool _isReady() = 0;

		private:

			/** Worker thread used for latched data source polling. */
			pthread_t _workThread;

			/** Whether polling is active. */
			bool _polling = false;

			/** Whether to stop polling. */
			bool _stop = false;

			/** Interval at which latched data is polled. In micro seconds. */
			long _pollingInterval;

			/** The last time the polling loop _finished_. */
			long _lastPollSec;
			long _lastPollUSec;

			/** The current latcher being used. */
			static Latcher* __currentLatcher;
	};
}

#endif