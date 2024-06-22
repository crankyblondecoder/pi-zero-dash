#ifndef PZD_INSTRUMENT_ON_OFF_H
#define PZD_INSTRUMENT_ON_OFF_H

#include <cstdint>
#include <sys/time.h>

#include "Instrument.hpp"

namespace piZeroDash
{
	/**
	 * Generic on/off switch instrument.
	 */
	class InstrumentOnOff : public Instrument
	{
		public:

			virtual ~InstrumentOnOff();
			InstrumentOnOff();

			// Impl.
			bool latch();

			/** Get the currently latched on/off state value. */
			bool getOnOffState();

			/** Produce test output. */
			void test();

		protected:

		private:

			/** Currently latched value. */
			bool _latchedValue;
	};
}

#endif