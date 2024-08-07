#ifndef PZD_GAUGE_HEADLIGHT_E36_H
#define PZD_GAUGE_HEADLIGHT_E36_H

#include "GaugeStdLight.hpp"
#include "InstrumentOnOff.hpp"

namespace piZeroDash
{
	/**
	 * E36 headlight gauge.
	 */
	class GaugeHeadlight_e36 : public GaugeStdLight
	{
		public:

			virtual ~GaugeHeadlight_e36();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeHeadlight_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

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

			/** Headlight low beam. */
			InstrumentOnOff _headLightLowBeamInstr;

			/** Last low beam value that foreground was drawn for. */
			bool _lastLowBeam = false;

			/** Headlight high beam. */
			InstrumentOnOff _headLightHighBeamInstr;

			/** Last high beam value that foreground was drawn for. */
			bool _lastHighBeam = false;

			/** background colour. */
			struct colour _backgroundColour = {0.5, 0.5, 0.5, 0.5};

			/** Lights off outline colour. */
			struct colour _offStrokeColour = {0.3, 0.3, 0.3, 1.0};;

			/** Lights on outline colour. */
			struct colour _onStrokeColour = {1.0, 1.0, 1.0, 0.5};

			/** Low beam foreground colour. */
			// green: 20, 227, 48
			struct colour _lowBeamForegroundColour = {20.0/255.0, 227.0/255, 48.0/255.0, 1.0};

			/** High beam foreground colour. */
			// 14, 14, 173
			struct colour _highBeamForegroundColour = {14.0/255.0, 14.0/255, 173.0/255.0, 1.0};

			// Keep track of the low/high beam active state for testing purposes.
			unsigned _testLowHighBeamState;

			/**
			 * @param cr Cairo context to draw to.
			 * @param strokeWidth Width of stroke lines.
			 * @param strokeColour Colour of stroked lines.
			 * @param beamDownSlantAngle Angle of beam lines (in radians). Front of line angles down.
			*/
			void __drawHeadlightOutline(cairo_t* cr, double strokeWidth, colour& strokeColour, double beamDownSlantAngle);
	};
}

#endif