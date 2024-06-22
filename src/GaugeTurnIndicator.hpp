#ifndef PZD_GAUGE_INDICATOR_H
#define PZD_GAUGE_INDICATOR_H

#include <cstdint>
#include <string>

#include "Gauge.hpp"
#include "InstrumentIndicator.hpp"

namespace piZeroDash
{
	/**
	 * Turn indicator gauge.
	 */
	class GaugeTurnIndicator : public Gauge
	{
		public:

			virtual ~GaugeTurnIndicator();

			/**
			 * @param left Make a left hand pointing arrow. Default is right.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTurnIndicator(bool left, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/**
			 * Draw a default turn indicator background.
			 * @param surface Surface to draw to.
			 * @param fillColour Colour to fill background in with.
			 */
			void _drawDefaultBackground(CairoSurface& surface, colour& fillColour);

			/**
			 * Draw a default turn indicator foreground.
			 * @param surface Surface to draw to.
			 * @param fillColour Colour to fill foreground in with.
			 */
			void _drawDefaultForeground(CairoSurface& surface, colour& fillColour);

		private:

			/** Whether a left hand pointing arrow is drawn. Default is right hand pointing. */
			bool _left;

			/** Generic turn indicator instrument. */
			InstrumentIndicator _indicatorInstr;

			/** Draw the path of the default arrow to a cairo context */
			void __drawDefaultPath(cairo_t* cr);
	};
}

#endif