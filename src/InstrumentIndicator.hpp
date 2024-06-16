#ifndef PZD_INSTRUMENT_INDICATOR_H
#define PZD_INSTRUMENT_INDICATOR_H

#include <cstdint>
#include <sys/time.h>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic indicator instrument.
	 */
	class InstrumentIndicator : public Instrument
	{
		public:

			virtual ~InstrumentIndicator();
			InstrumentIndicator();

			// Impl.
			bool latch();

			enum IndicatorState
			{
				NONE,
				LEFT,
				RIGHT,
				BOTH
			};

			/** Get the currently latched indicator value. */
			IndicatorState getIndicatorState();

			/** Produce test output. */
			void test(unsigned numberCycles);

			/** Get whether this instrument is currently in test mode. */
			bool inTestMode();

		protected:

		private:

			/** Currently latched value. */
			IndicatorState _latchedValue;

			/** Whether this instrument is in test mode. */
			bool _inTestMode;

			/** The number of cycles that need to be completed during test mode. */
			unsigned _curTestNumberCycles;

			/** The current cycle number during test mode. */
			unsigned _curTestCycleNumber;

			/** Current test value. */
			IndicatorState _curTestValue;

			/** The time of the last test latch. */
			long _testLastSec;
			long _testLastUSec;

			/** Whether testing is in single step mode */
			bool _testSingleStep;
	};
}

#endif