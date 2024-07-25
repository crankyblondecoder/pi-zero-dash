#ifndef PZD_GAUGE_SPEEDO_H
#define PZD_GAUGE_SPEEDO_H

#include "GaugeDial.hpp"
#include "InstrumentSpeedo.hpp"

namespace piZeroDash
{
	/**
	 * Generic speedo gauge.
	 */
	class GaugeSpeedo : public GaugeDial
	{
		public:

			virtual ~GaugeSpeedo();

			/**
			 * @param radius Radius of dial.
			 * @param dialCentreX Centre of dial, x coordinate.
			 * @param dialCentreY Centre of dial, y coordinate.
			 * @param maxSpeed Maximum speed displayed by speedo.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeSpeedo(double radius, double dialCentreX, double dialCentreY, unsigned maxSpeed, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			/** Get the maximum speed displayed. */
			unsigned _getMaxSpeed();

			/**
			 * Draw the default speedo background.
			 * @param surface Surface to draw to.
			 */
			void _drawDefaultBackground(CairoSurface& surface);

			/**
			 * Set all speedo visual properties.
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
			 * @param unitsLableText The text to display to indicate the units of the dial.
			 */
			void _setProperties(double markedSpeedFontSize, colour& markedSpeedFontColour,
				unsigned markedSpeedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour, colour& preciseSpeedBackgroundColour,
				double preciseSpeedBackgroundWidth, double preciseSpeedBackgroundHeight, string unitsLabelText);

			/**
			 * Draw the default speedo forground.
			 * @param surface Surface to draw to.
			 * @param indicatorLineLength Length of speed indicator line.
			 * @param indicatorLineWidth Width of speed indicator line.
			 * @param indicatorLineColour Colour of indicator line.
			 */
			void _drawDefaultForeground(CairoSurface& surface, double indicatorLineLength, double indicatorLineWidth,
				colour& indicatorLineColour, double preciseSpeedFontSize, colour& preciseSpeedFontColour);

			/**
			 * Calculate the precise speed box bounds in local coordintes.
			 * @param width Width of box.
			 * @param height Height of box.
			 */
			bounds _calcPreciseSpeedBoxBounds(double width, double height);

			// Impl.
			bool _requiresDrawForeground(Instrument* instrument);

		private:

			/** Last speed that foreground was drawn for. */
			unsigned _lastSpeed = 1 << 30;

			/** Generic speedo instrument. */
			InstrumentSpeedo _speedoInstr;

			/** Maximum speed displayed by the speedo. */
			unsigned _maxSpeed;

			/** Colour of the precise speed background. */
			colour _preciseSpeedBackgroundColour;

			/** Width of precise speed background box. */
			double _preciseSpeedBackgroundWidth;

			/** Height of precise speed background box. */
			double _preciseSpeedBackgroundHeight;

			/** The text to display that represents the units. */
			string _unitsLabelText;

			/** Indicated line length. */
			double _lineLength;

			/** Offset of the start of the indicated line back towards the dial centre. */
			double _lineStartOffset;

			/** Size of the marked speed text. */
			double _markedSpeedFontSize;

			/** Colour of the marked speed text. */
			colour _markedSpeedFontColour;
	};
}

#endif