#ifndef PZD_INSTRUMENT_SPEEDO_H
#define PZD_INSTRUMENT_SPEEDO_H

#include <cstdint>

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

			bool _inTestMode;

			/** The maximum speed allowed during test mode. */
			unsigned _curTestMaxSpeed;

			/** If true the test value is increasing. */
			bool _curTestForwardDirection;

			/** Current test value. */
			unsigned _curTestValue;
	};
}

#endif