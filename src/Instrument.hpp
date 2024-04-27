#ifndef PZD_INSTRUMENT_H
#define PZD_INSTRUMENT_H

#include <cstdint>

namespace piZeroDash
{
	/**
	 * Instruments read and interpret data from one or more physical sources.
	 */
	class Instrument
	{
		public:

			virtual ~Instrument(){};
			Instrument(){};

			/**
			 * Latch the current state of the instrument.
			 * This means to store it locally for later retrieval.
			 * @returns True if the latched data is different from the previously latched data.
			 */
			virtual bool latch() = 0;

		protected:

		private:
	};
}

#endif