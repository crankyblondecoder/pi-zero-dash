#ifndef PZD_INSTRUMENT_TACHO_H
#define PZD_INSTRUMENT_TACHO_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic tacho instrument.
	 */
	class InstrumentTacho : public Instrument
	{
		public:

			virtual ~InstrumentTacho();
			InstrumentTacho();

			// Impl.
			bool latch();

			/** Get the currently latched rpm. */
			unsigned getRpm();

			/** Run a test cycle. */
			void test(unsigned maxRpm);

		protected:

		private:

			/** Currently latched value. */
			unsigned _latchedValue;
	};
}

#endif