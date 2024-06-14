#ifndef PZD_GAUGE_H
#define PZD_GAUGE_H

#include <cstdint>

#include "Instrument.hpp"
#include "Visual.hpp"

#define NUM_INSTRUMENTS 16

namespace piZeroDash
{
	/**
	 * Gauges display data from one or more instruments.
	 */
	class Gauge : public Visual
	{
		public:

			virtual ~Gauge();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			Gauge(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			/**
			 * Latch instrument(s) and write gauges foreground as a result if any latched values change.
			 */
			void scan();

			/** Run a test cycle. */
			virtual void test() = 0;

			/** Whether this gauge is currentlyu in a test cycle. */
			virtual bool inTestMode() = 0;

		protected:

			/**
			 * Add an instrument to this gauge that can be read to get the state to display.
			 * @param instrument Instrument to add. _Not_ owned by this.
			 */
			void _addInstrument(Instrument* instrument);

			/**
			 * Draw a default dial indicator background.
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
			void _drawDefaultDialIndicatorBackground(CairoSurface& surface, int startNumber, int endNumber,
				double markDistance, bool showMinor, bool showPriorMinor, bool showLaterMinor,
				double markedFontSize, colour& markedFontColour, unsigned markedFontDecimalPlaces,
				double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset,
				colour& majorLineColour, colour& minorLineColour);

		private:

			unsigned _instrumentCount;

			Instrument** _instruments;
	};
}

#endif