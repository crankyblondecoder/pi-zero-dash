#ifndef PZD_GAUGE_H
#define PZD_GAUGE_H

#include <cstdint>

#include "Visual.hpp"

namespace piZeroDash
{
	/**
	 * Gauges display data from one or more instruments.
	 */
	class Gauge : public Visual
	{
		public:

			virtual ~Gauge(){};

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			Gauge(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

		protected:

		private:
	};
}

#endif