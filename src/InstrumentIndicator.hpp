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
			void test();

		protected:

		private:

			/** Currently latched value. */
			IndicatorState _latchedValue;
	};
}

#endif