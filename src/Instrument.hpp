#ifndef PZD_INSTRUMENT_H
#define PZD_INSTRUMENT_H

#include <cstdint>
#include <sys/time.h>

namespace piZeroDash
{
	/**
	 * Instruments read and interpret data from one or more physical sources.
	 */
	class Instrument
	{
		public:

			virtual ~Instrument();
			Instrument();

			/**
			 * Latch the current state of the instrument.
			 * This means to store it locally for later retrieval.
			 * @returns True if the latched data is different from the previously latched data.
			 */
			virtual bool latch() = 0;

			/** Get whether this instrument is currently in test mode. */
			bool inTestMode();

		protected:

			/** Get whether this instrument is currently in test mode. */
			bool _getInTestMode();

			/**
			 * Start numerical test output cycle.
			 * @param min Minimum test value.
			 * @param max Maximum test value.
			 * @param testStepPerMillis Test step per millisecond.
			 * @param forwardOnly If true only produce increasing values until the max is reached.
			 */
			void _testNumerical(double min, double max, double testStepPerMillis, bool forwardOnly);

			/**
			 * Get a numerical test value.
			 */
			double _getNumericalTestValue();

		private:

			/** Whether this instrument is in test mode. */
			bool _inTestMode = false;

			/** The minimum numerical value allowed during test mode. */
			double _numTestMin;

			/** The maximum numerical value allowed during test mode. */
			double _numTestMax;

			/** Numerical test step per millisecond. */
			double _numTestStep;

			/** Whether the numerical test only produces increasing values. */
			bool _numTestForwardOnly;

			/** If true the numerical test value is increasing. Otherwise it is (optionally) decreasing. */
			bool _numTestForwardDirection;

			/** Current numerical test value. */
			double _curNumTestValue;

			/** The time at the beginning of the test cycle. */
			long _testStartSec;
			long _testStartUSec;

			/** The time of the last test latch. */
			long _testLastSec;
			long _testLastUSec;

			/** Whether testing is in single step mode */
			bool _testSingleStep;
	};
}

#endif