#ifndef PZD_GAUGE_DIAL_H
#define PZD_GAUGE_DIAL_H

#include <cstdint>

#include "Gauge.hpp"

namespace piZeroDash
{
	/**
	 * Generic dial gauge.
	 */
	class GaugeDial : public Gauge
	{
		public:

			virtual ~GaugeDial();

			/**
			 * @param radius Radius of dial.
			 * @param dialCentreX Centre of dial, x coordinate.
			 * @param dialCentreY Centre of dial, y coordinate.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeDial(double radius, double dialCentreX, double dialCentreY, int globalPositionX, int globalPositionY,
				unsigned width, unsigned height);

		protected:

			/**
			 * Draw a default dial gauge background.
			 * @param surface Surface to draw to.
			 * @param startNumber First number shown at the horizontal.
			 * @param endNumber Last number shown at the horizontal.
			 * @param markDistance Distance between marks (including minor marks).
			 * @param showMinor Whether to show minor marks.
			 * @param showPriorMinor Whether to show a minor mark prior to the first major indicator mark.
			 * @param showLaterMinor Whether to show a minor mark after the last major indicator mark.
			 * @param markedFontSize The font size of the numbers that appear near the marked lines.
			 * @param markedFontColour Colour of the numbers under the marked lines.
			 * @param markedFontDecimalPlaces Number of decimal places to show on marked numbers.
			 * @param lineLength The length of the marked lines.
			 * @param majorLineWidth Major marked line width.
			 * @param minorLineWidth Minor marked line width.
			 * @param lineStartOffset Amount to move line back towards gauge centre.
			 * @param majorLineColour Colour of the major marked lines.
			 * @param minorLineColour Colour of the minor marked lines.
			 */
			void _drawDefaultBackground(CairoSurface& surface, int startNumber, int endNumber,
				double markDistance, bool showMinor, bool showPriorMinor, bool showLaterMinor,
				double markedFontSize, colour& markedFontColour, unsigned markedFontDecimalPlaces,
				double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset,
				colour& majorLineColour, colour& minorLineColour);

			/** Get dial's radius. */
			double _getRadius();

			/** Get the x coordinate of the centre of the dial.*/
			double _getDialCentreX();

			/** Get the y coordinate of the centre of the dial.*/
			double _getDialCentreY();

		private:

			/** Radius of dial. */
			double _radius;

			/** Centre of dial, x coordinate. */
			double _dialCentreX;

			/** Centre of dial, y coordinate. */
			double _dialCentreY;
	};
}

#endif