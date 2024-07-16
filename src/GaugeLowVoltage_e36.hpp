#ifndef PZD_GAUGE_LOW_VOLTAGE_E36_H
#define PZD_GAUGE_LOW_VOLTAGE_E36_H

#include "GaugeStdLight.hpp"
#include "InstrumentVoltage.hpp"

namespace piZeroDash
{
	/**
	 * E36 low voltage gauge.
	 */
	class GaugeLowVoltage_e36 : public GaugeStdLight
	{
		public:

			virtual ~GaugeLowVoltage_e36();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeLowVoltage_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

			// Impl.
			bool _requiresDrawForeground(Instrument* instrument);

		private:

			/** Reads current voltage. */
			InstrumentVoltage _voltageInstr;

			/** Last voltage that foreground was drawn for. */
			double _lastVoltage = 1 << 30;

			double _lowVoltageThreshold = 12.7;

			/** background colour. */
			struct colour _backgroundColour = {0.5, 0.5, 0.5, 0.5};

			/** Voltage okay outline colour. */
			struct colour _offStrokeColour = {0.3, 0.3, 0.3, 1.0};

			/** Voltage not okay outline colour. */
			struct colour _onStrokeColour = {1.0, 0.0, 0.0, 1.0};

			/** Park brake on foreground colour. */
			struct colour _onForegroundColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.9};

			/**
			 * @param cr Cairo context to draw to.
			 * @param strokeWidth Width of stroke lines.
			 * @param strokeColour Colour of stroked lines.
			*/
			void __drawLowVoltageOutline(cairo_t* cr, double strokeWidth, colour& strokeColour);
	};
}

#endif