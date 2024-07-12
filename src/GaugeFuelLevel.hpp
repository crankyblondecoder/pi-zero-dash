#ifndef PZD_GAUGE_FUEL_LEVEL_H
#define PZD_GAUGE_FUEL_LEVEL_H

#include "GaugeDial.hpp"
#include "InstrumentFuelLevel.hpp"

namespace piZeroDash
{
	/**
	 * Generic fuel level gauge.
	 */
	class GaugeFuelLevel : public GaugeDial
	{
		public:

			virtual ~GaugeFuelLevel();

			/**
			 * @param maxFuelLevel Maximum fuel level displayed.
			 * @param radius Radius of dial.
			 * @param dialCentreX Centre of dial, x coordinate.
			 * @param dialCentreY Centre of dial, y coordinate.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeFuelLevel(int maxFuelLevel, double radius, double dialCentreX, double dialCentreY, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

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
			 * @param startAngle Start angle of the first major mark. Clockwise from x axis in radians.
			 * @param endAngle End angle of the last major mark. Clockwise from x axis in radians. If this is less than the
			 *        start angle the dial direction is "reversed".
			 * @param lowFuelLevel Fuel level below which triggers the low fuel indication.
			 * @param lowFuelLevelIndicatorColour Colour displayed for indicator when low fuel level is reached.
			 */
			void _setStandardProperties(double markedFontSize, colour& markedFontColour,
				unsigned markedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour, double startAngle, double endAngle,
				double lowFuelLevel, colour& lowFuelLevelIndicatorColour);

			/**
			 * Draw the default forground.
			 * @param surface Surface to draw to.
			 * @param indicatorLineLength Length of speed indicator line.
			 * @param indicatorLineWidth Width of speed indicator line.
			 * @param indicatorLineColour Colour of indicator line.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
				colour& indicatorLineColour);

		private:

			/** Generic fuel level instrument. */
			InstrumentFuelLevel _fuelLevelInstr;

			/** Maximum fuel level displayed. */
			int _maxFuelLevel;

			/** Predefined standard radial sections to use for foreground. */
			IndicatorRadialSection _standardRadialSections[1];

			/** Length of the marked (background) lines. */
			double _lineLength;
	};
}

#endif