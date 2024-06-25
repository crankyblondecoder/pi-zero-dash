#ifndef PZD_INSTRUMENT_ENGINE_TEMP_H
#define PZD_INSTRUMENT_ENGINE_TEMP_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic tacho instrument.
	 */
	class InstrumentEngineTemp : public Instrument
	{
		public:

			virtual ~InstrumentEngineTemp();
			InstrumentEngineTemp();

			// Impl.
			bool latch();

			/** Get the currently latched engine temperature. */
			int getEngineTemp();

			/** Run a test cycle. */
			void test(int minTemp, int maxTemp);

		protected:

		private:

			/** Currently latched value. */
			int _latchedValue;
	};
}

#endif