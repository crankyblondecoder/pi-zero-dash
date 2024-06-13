#ifndef PZD_GAUGE_TACHO_H
#define PZD_GAUGE_TACHO_H

#include "Gauge.hpp"
#include "InstrumentSpeedo.hpp"

namespace piZeroDash
{
	/**
	 * Generic tachometer gauge.
	 */
	class GaugeTacho : public Gauge
	{
		public:

			virtual ~GaugeTacho();

			/**
			 * @param maxSpeed Maximum RPM displayed by tacho.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTacho(unsigned maxRpm, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/** Get the maximum RPM displayed. */
			unsigned _getMaxRpm();

			/**
			 * Draw the default tacho background.
			 * @param surface Surface to draw to.
			 * @param markedRpmFontSize Size of font to indicate RPM.
			 * @param markedRpmFontColour Colour of font to indicate RPM.
			 * @param lineLength Length of radial line that delinates indicated RPM.
			 * @param lineWidth Width of radial line.
			 * @param lineColour Colour of radial line.
			 */
			void _drawDefaultBackground(CairoSurface& surface, double markedRpmFontSize, colour& markedRpmFontColour,
				double lineLength, double lineWidth, colour& lineColour);

			/**
			 * Draw the default tacho forground.
			 * @param surface Surface to draw to.
			 * @param redlineWarningThreshold RPM after which redline warning is displayed.
			 * @param redline RPM after which redline is displayed.
			 * @param radialSectionLength Length of radial section that displays filled colour to indicate current RPM.
			 * @param normalColour The normal below redline threshold colour.
			 * @param redlineWarningThresholdColour Colour to display for redline threshold. ie Just prior to redline.
			 * @param redlineColour Colour to display for redline reached and/or exceeded.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double redlineWarningThreshold, double redline,
				double radialSectionLength, colour& normalColour, colour& redlineWarningThresholdColour,
				colour& redlineColour);

		private:

			/** Generic tacho instrument. */
			InstrumentTacho _tachoInstr;

			/** Maximum RPM displayed by the tacho. */
			unsigned _maxRpm;
	};
}

#endif