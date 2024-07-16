#ifndef PZD_GAUGE_TACHO_H
#define PZD_GAUGE_TACHO_H

#include "GaugeDial.hpp"
#include "InstrumentTacho.hpp"

namespace piZeroDash
{
	/**
	 * Generic tachometer gauge.
	 */
	class GaugeTacho : public GaugeDial
	{
		public:

			virtual ~GaugeTacho();

			/**
			 * @param maxSpeed Maximum RPM displayed by tacho.
			 * @param redlineRpm RPM value of redline.
			 * @param redlineWarningRpm RPM value of approaching redline warning.
			 * @param flashRedline Whether to flash the tacho when redline is exceeded.
			 * @param radius Radius of dial.
			 * @param dialCentreX Centre of dial, x coordinate.
			 * @param dialCentreY Centre of dial, y coordinate.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTacho(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, bool flashRedline, double radius,
				double dialCentreX, double dialCentreY, int globalPositionX, int globalPositionY, unsigned width,
				unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/** Get the maximum RPM displayed. */
			unsigned _getMaxRpm();

			/**
			 * Set all tacho visual properties.
			 * @param markedRpmFontSize The font size of the numbers that appear near the marked lines.
			 * @param markedRpmFontColour Colour of the numbers under the marked lines.
			 * @param lineLength The length of the marked lines.
			 * @param majorLineWidth Major marked line width.
			 * @param minorLineWidth Minor marked line width.
			 * @param lineStartOffset Amount to move line back towards gauge centre.
			 * @param majorLineColour Colour of the major marked lines.
			 * @param minorLineColour Colour of the minor marked lines.
			 * @param normalColour The normal below redline threshold colour.
			 * @param redlineWarningThresholdColour Colour to display for redline threshold. ie Just prior to redline.
			 * @param redlineColour Colour to display for redline reached and/or exceeded.
			 */
			void _setProperties(double markedRpmFontSize, colour& markedRpmFontColour,
				unsigned markedRpmFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour, colour& normalColour,
				colour& redlineWarningThresholdColour, colour& redlineColour);

			/**
			 * Draw the default tacho forground.
			 * @param surface Surface to draw to.
			 * @param sectionRadialLength Length of section, in radial direction, that displays filled colour to indicate
			 *        current RPM.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double sectionRadialLength);

			// Impl.
			bool _requiresDrawForeground(Instrument* instrument);

		private:

			/** Generic tacho instrument. */
			InstrumentTacho _tachoInstr;

			/** Last RPM that foreground was drawn for. */
			unsigned _lastRpm = 1 << 30;

			/** Maximum RPM displayed by the tacho. */
			unsigned _maxRpm;

			/** Redline RPM value. */
			unsigned _redlineRpm;

			/** Redline warning threshold RPM */
			unsigned _redlineWarningRpm;

			/** Whether to flash whole of indicated sections when current rpm enters redline section. */
			bool _flashRedline;

			/** Predefined standard tacho radial sections to use for foreground. */
			IndicatorRadialSection _standardRadialSections[3];
	};
}

#endif