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

			/** Get the currently latched volume of fuel. */
			double getFuelVolume();

			/** Run a test cycle. */
			void test(unsigned maxFuelLitres);

		protected:

		private:

			/** Currently latched value. */
			double _latchedValue;
	};
}

#endif