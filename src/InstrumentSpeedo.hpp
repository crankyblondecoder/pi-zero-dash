#ifndef PZD_INSTRUMENT_SPEEDO_H
#define PZD_INSTRUMENT_SPEEDO_H

#include <cstdint>
#include <sys/time.h>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic speedo instrument.
	 */
	class InstrumentSpeedo : public Instrument
	{
		public:

			virtual ~InstrumentSpeedo();
			InstrumentSpeedo();

			// Impl.
			bool latch();

			/** Get the currently latched speed. */
			unsigned getSpeed();

			/** Produce test output. */
			void test(unsigned maxSpeed);

			/** Get whether this instrument is currently in test mode. */
			bool inTestMode();

		protected:

		private:

			/** Currently latched value. */
			unsigned _latchedValue;

			/** Whether this instrument is in test mode. */
			bool _inTestMode;

			/** The maximum speed allowed during test mode. */
			unsigned _curTestMaxSpeed;

			/** If true the test value is increasing. */
			bool _curTestForwardDirection;

			/** Current test value. */
			unsigned _curTestValue;

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