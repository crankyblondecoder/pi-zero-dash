#ifndef PZD_GAUGE_PARK_BRAKE_E36_H
#define PZD_GAUGE_PARK_BRAKE_E36_H

#include "GaugeStdLight.hpp"
#include "InstrumentOnOff.hpp"

namespace piZeroDash
{
	/**
	 * E36 park brake gauge.
	 */
	class GaugeParkBrake_e36 : public GaugeStdLight
	{
		public:

			virtual ~GaugeParkBrake_e36();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeParkBrake_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

		private:

			/** Park brake on/off. */
			InstrumentOnOff _parkBrakeOnOffInstr;

			/** background colour. */
			struct colour _backgroundColour = {0.5, 0.5, 0.5, 0.5};

			/** Park brake off outline colour. */
			struct colour _offStrokeColour = {0.3, 0.3, 0.3, 1.0};;

			/** Park brake on outline colour. */
			struct colour _onStrokeColour = {1.0, 0.0, 0.0, 1.0};

			/** Park brake on foreground colour. */
			struct colour _onForegroundColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.9};

			/**
			 * @param cr Cairo context to draw to.
			 * @param strokeWidth Width of stroke lines.
			 * @param strokeColour Colour of stroked lines.
			*/
			void __drawParkBrakeOutline(cairo_t* cr, double strokeWidth, colour& strokeColour);
	};
}

#endif