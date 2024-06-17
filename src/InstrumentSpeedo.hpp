#ifndef PZD_INSTRUMENT_SPEEDO_H
#define PZD_INSTRUMENT_SPEEDO_H

#include <cstdint>
#include <sys/time.h>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic speedo instrument.
	 */
	class InstrumentSpeedo : public Instrument
	{
		public:

			virtual ~InstrumentSpeedo();
			InstrumentSpeedo();

			// Impl.
			bool latch();

			/** Get the currently latched speed. */
			unsigned getSpeed();

			/** Produce test output. */
			void test(unsigned maxSpeed);

		protected:

		private:

			/** Currently latched value. */
			unsigned _latchedValue;
	};
}

#endif