#ifndef PZD_INSTRUMENT_OIL_PRESSURE_H
#define PZD_INSTRUMENT_OIL_PRESSURE_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic engine oil pressure instrument.
	 */
	class InstrumentOilPressure : public Instrument
	{
		public:

			virtual ~InstrumentOilPressure();
			InstrumentOilPressure();

			// Impl.
			bool latch();

			/** Get the currently latched oil pressure. */
			double getOilPressure();

			/** Run a test cycle. */
			void test(double maxOilPressure);

		protected:

		private:

			/** Currently latched value. */
			double _latchedValue;
	};
}

#endif