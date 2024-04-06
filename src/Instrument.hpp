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

		protected:

		private:
	};
}

#endif