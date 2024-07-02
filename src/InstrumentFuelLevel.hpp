#ifndef PZD_INSTRUMENT_FUEL_LEVEL_H
#define PZD_INSTRUMENT_FUEL_LEVEL_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic fuel level instrument.
	 */
	class InstrumentFuelLevel : public Instrument
	{
		public:

			virtual ~InstrumentFuelLevel();
			InstrumentFuelLevel();

			// Impl.
			bool latch();

			/** Get the currently latched amount of fuel in litres. */
			unsigned getFuelLitres();

			/** Run a test cycle. */
			void test(unsigned maxFuelLitres);

		protected:

		private:

			/** Currently latched value. */
			unsigned _latchedValue;
	};
}

#endif