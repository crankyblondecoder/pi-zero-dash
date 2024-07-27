#ifndef PZD_INSTRUMENT_OIL_TEMP_H
#define PZD_INSTRUMENT_OIL_TEMP_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic engine oil temperature instrument.
	 */
	class InstrumentOilTemperature : public Instrument
	{
		public:

			virtual ~InstrumentOilTemperature();
			InstrumentOilTemperature();

			// Impl.
			bool latch();

			/** Get the currently latched oil temperature. */
			double getOilTemperature();

			/** Run a test cycle. */
			void test(double maxOilTemperature);

		protected:

		private:

			/** Currently latched value. */
			double _latchedValue;
	};
}

#endif