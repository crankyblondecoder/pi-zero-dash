#ifndef PZD_GAUGE_H
#define PZD_GAUGE_H

#include <cstdint>

#include "Visual.hpp"

namespace piZeroDash
{
	/**
	 * Gauges display data from one or more instruments.
	 */
	class Gauge : Visual
	{
		public:

			virtual ~Gauge(){};
			Gauge(){};

		protected:

		private:
	};
}

#endif