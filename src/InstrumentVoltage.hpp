#ifndef PZD_INSTRUMENT_VOLTAGE_H
#define PZD_INSTRUMENT_VOLTAGE_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic power supply voltage instrument.
	 */
	class InstrumentVoltage : public Instrument
	{
		public:

			virtual ~InstrumentVoltage();
			InstrumentVoltage();

			// Impl.
			bool latch();

			/** Get the currently latched voltage. */
			double getVoltage();

			/** Run a test cycle. */
			void test(double maxVoltage);

		protected:

		private:

			/** Currently latched value. */
			double _latchedValue;
	};
}

#endif