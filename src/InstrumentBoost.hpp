#ifndef PZD_INSTRUMENT_BOOST_H
#define PZD_INSTRUMENT_BOOST_H

#include <cstdint>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic forced induction boost instrument.
	 * Defaults to the units of PSI.
	 */
	class InstrumentBoost : public Instrument
	{
		public:

			virtual ~InstrumentBoost();
			InstrumentBoost();

			// Impl.
			bool latch();

			/** Get the currently latched amount of boost in psi. */
			double getBoost();

			/** Run a test cycle. */
			void test(double minBoost, double maxBoost);

		protected:

		private:

			/** Currently latched value. */
			double _latchedValue;
	};
}

#endif