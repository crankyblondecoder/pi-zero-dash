#ifndef PZD_GAUGE_STD_LIGHT_H
#define PZD_GAUGE_STD_LIGHT_H

#include <cstdint>
#include <string>

#include "Gauge.hpp"

namespace piZeroDash
{
	/**
	 * Base class of standard light gauge.
	 */
	class GaugeStdLight : public Gauge
	{
		public:

			virtual ~GaugeStdLight();

			/**
			 * @param cornerRadius The radius of the corner of the drawn box.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeStdLight(double cornerRadius, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

		protected:

			/** Draw the path of the default box to a cairo context */
			void _drawDefaultPath(cairo_t* cr);

		private:

			/** The radius of the corner of the drawn box. */
			double _cornerRadius;
	};
}

#endif