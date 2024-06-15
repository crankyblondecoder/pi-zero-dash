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
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTacho(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/** Get the maximum RPM displayed. */
			unsigned _getMaxRpm();

			/**
			 * Draw a default tacho background.
			 * @param surface Surface to draw to.
			 * @param markedRpmFontSize The font size of the numbers that appear near the marked lines.
			 * @param markedRpmFontColour Colour of the numbers under the marked lines.
			 * @param lineLength The length of the marked lines.
			 * @param majorLineWidth Major marked line width.
			 * @param minorLineWidth Minor marked line width.
			 * @param lineStartOffset Amount to move line back towards gauge centre.
			 * @param majorLineColour Colour of the major marked lines.
			 * @param minorLineColour Colour of the minor marked lines.
			 */
			void _drawDefaultBackground(CairoSurface& surface, double markedRpmFontSize, colour& markedRpmFontColour,
				unsigned markedRpmFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour);

			/**
			 * Draw the default tacho forground.
			 * @param surface Surface to draw to.
			 * @param radialSectionLength Length of radial section that displays filled colour to indicate current RPM.
			 * @param normalColour The normal below redline threshold colour.
			 * @param redlineWarningThresholdColour Colour to display for redline threshold. ie Just prior to redline.
			 * @param redlineColour Colour to display for redline reached and/or exceeded.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double radialSectionLength, colour& normalColour,
				colour& redlineWarningThresholdColour, colour& redlineColour);

		private:

			/** Generic tacho instrument. */
			InstrumentTacho _tachoInstr;

			/** Maximum RPM displayed by the tacho. */
			unsigned _maxRpm;

			/** Redline RPM value. */
			unsigned _redlineRpm;

			/** Redline warning threshold RPM */
			unsigned _redlineWarningRpm;
	};
}

#endif