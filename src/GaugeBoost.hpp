#ifndef PZD_GAUGE_BOOST_H
#define PZD_GAUGE_BOOST_H

#include "GaugeDial.hpp"
#include "InstrumentBoost.hpp"

namespace piZeroDash
{
	/**
	 * Generic forced induction boost gauge.
	 */
	class GaugeBoost : public GaugeDial
	{
		public:

			virtual ~GaugeBoost();

			/**
			 * @param minBoost Minimum boost displayed.
			 * @param maxBoost Maximum boost displayed.
			 * @param neutralBoost The boost value that is considered the changeover value from "not effective" to "effective"
			 *        boost. Can be associated with a colour change.
			 * @param radius Radius of dial.
			 * @param dialCentreX Centre of dial, x coordinate.
			 * @param dialCentreY Centre of dial, y coordinate.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeBoost(int minBoost, int maxBoost, int neutralBoost,
				double radius, double dialCentreX, double dialCentreY, int globalPositionX, int globalPositionY,
				unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/**
			 * Set all standard boost visual properties.
			 * @param markedFontSize The font size of the numbers that appear near the marked lines.
			 * @param markedFontColour Colour of the numbers under the marked lines.
			 * @param markedFontDecimalPlaces Number of decimal places to display.
			 * @param lineLength The length of the marked lines.
			 * @param majorLineWidth Major marked line width.
			 * @param minorLineWidth Minor marked line width.
			 * @param lineStartOffset Amount to move line back towards gauge centre.
			 * @param majorLineColour Colour of the major marked lines.
			 * @param minorLineColour Colour of the minor marked lines.
			 * @param preNeutralColour The below neutral boost colour.
			 * @param postNeutralColour The above neutral boost colour.
			 * @param startAngle Start angle of the first major mark. Clockwise from x axis in radians.
			 * @param endAngle End angle of the last major mark. Clockwise from x axis in radians. If this is less than the
			 *        start angle the dial direction is "reversed".
			 */
			void _setStandardProperties(double markedFontSize, colour& markedFontColour,
				unsigned markedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour, colour& preNeutralColour,
				colour& postNeutralColour, double startAngle, double endAngle);

			/**
			 * Draw the default forground.
			 * @param surface Surface to draw to.
			 * @param sectionRadialLength Length of section, in radial direction, that displays filled colour to indicate
			 *        current value.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double sectionRadialLength);

		private:

			/** Generic boost instrument. */
			InstrumentBoost _boostInstr;

			/** Minimum boost displayed. */
			int _minBoost;

			/** Maximum boost displayed. */
			int _maxBoost;

			/**
			 * The boost value that is considered the changeover value from "not effective" to "effective" boost. Can be
			 * associated with a colour change.
			 */
			int _neutralBoost;

			/** Predefined standard radial sections to use for foreground. */
			IndicatorRadialSection _standardRadialSections[2];
	};
}

#endif