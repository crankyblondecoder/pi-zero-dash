#ifndef PZD_GAUGE_SPEEDO_H
#define PZD_GAUGE_SPEEDO_H

#include "Gauge.hpp"
#include "InstrumentSpeedo.hpp"

namespace piZeroDash
{
	/**
	 * Generic speedo gauge.
	 */
	class GaugeSpeedo : public Gauge
	{
		public:

			virtual ~GaugeSpeedo();

			/**
			 * @param maxSpeed Maximum speed displayed by speedo.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeSpeedo(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/** Get the maximum speed displayed. */
			unsigned _getMaxSpeed();

			/**
			 * Draw a default speedo background.
			 * @param surface Surface to draw to.
			 * @param markedSpeedFontSize The font size of the numbers that appear near the marked lines.
			 * @param markedSpeedFontColour Colour of the numbers under the marked lines.
			 * @param lineLength The length of the marked lines.
			 * @param majorLineWidth Major marked line width.
			 * @param minorLineWidth Minor marked line width.
			 * @param lineStartOffset Amount to move line back towards gauge centre.
			 * @param majorLineColour Colour of the major marked lines.
			 * @param minorLineColour Colour of the minor marked lines.
			 * @param preciseSpeedBackgroundColour Colour of the precise speed background.
			 * @param preciseSpeedBackgroundWidth Width of precise speed background box.
			 * @param preciseSpeedBackgroundHeight Height of precise speed background box.
			 */
			void _drawDefaultBackground(CairoSurface& surface, double markedSpeedFontSize, colour& markedSpeedFontColour,
				double lineLength, double majorLineWidth, double minorLineWidth, double lineStartOffset,
				colour& majorLineColour, colour& minorLineColour, colour& preciseSpeedBackgroundColour,
				double preciseSpeedBackgroundWidth, double preciseSpeedBackgroundHeight);

			/**
			 * Draw the default speedo forground.
			 * @param surface Surface to draw to.
			 * @param indicatorLineLength Length of speed indicator line.
			 * @param indicatorLineWidth Width of speed indicator line.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth);

		private:

			/** Generic speedo instrument. */
			InstrumentSpeedo _speedoInstr;

			/** Maximum speed displayed by the speedo. */
			unsigned _maxSpeed;
	};
}

#endif