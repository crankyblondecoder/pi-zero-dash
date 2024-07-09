#ifndef PZD_GAUGE_DIAL_H
#define PZD_GAUGE_DIAL_H

#include <cstdint>
#include <string>

#include "Gauge.hpp"

using namespace std;

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

			struct IndicatorRadialSection
			{
				/**
				 * Indicated value this section starts displaying from, including. Being greater or equal to this value
				 * also triggers flashing if the flash flag is set.
				 */
				double indicatedValueStart;

				/** Indicated value this section displays up to but not including. */
				double indicatedValueEnd;

				/** Colour of section. */
				colour sectionColour;

				/** Whether this section flashes. */
				bool flash;
			};

		protected:

			/**
			 * Set standard dial gauge properties.
			 * @param startNumber First number shown at the start angle.
			 * @param endNumber Last number shown at the end angle.
			 * @param markDistance Numerical distance between marks (including minor marks). ie How much the marked number
			 *        changes. e.g. 10 -> 20 -> 30 ...
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
			 * @param startAngle Start angle of the first major mark. Clockwise from x axis in radians.
			 * @param endAngle End angle of the last major mark. Clockwise from x axis in radians. If this is less than the
			 *        start angle the dial direction is "reversed".
			 */
			void _setStandardProperties(double startNumber, double endNumber, double markDistance, bool showMinor,
				bool showPriorMinor, bool showLaterMinor, double markedFontSize, colour& markedFontColour,
				unsigned markedFontDecimalPlaces, double lineLength, double majorLineWidth, double minorLineWidth,
				double lineStartOffset, colour& majorLineColour, colour& minorLineColour, double startAngle, double endAngle);

			/**
			 * Calculate the precise value box bounds in local coordinates.
			 * @param width Width of box.
			 * @param height Height of box.
			 */
			bounds _calcPreciseValueBoxBounds(double width, double height);

			/**
			 * Draw a default dial gauge background.
			 * @param surface Surface to draw to.
			 */
			void _drawDefaultBackground(CairoSurface& surface);

			/**
			 * Draw the standard precise value box background.
			 * @param surface Surface to draw to.
			 * @param preciseValueBoxWidth Width of the box drawn.
			 * @param preciseValueBoxHeight Height of the box drawn.
			 * @param preciseValueBoxColour Colour of the box drawn.
			 */
			void _drawStandardPreciseBoxBackground(CairoSurface& surface, double preciseValueBoxWidth,
				double preciseValueBoxHeight, colour& preciseValueBoxColour);

			/**
			 * Draw the standard precise value box foreground.
			 * @param surface Surface to draw to.
			 * @param preciseValue Precise value to display.
			 * @param numDecimalPoints Number of decimal points to display.
			 * @param unitString String representation of the units of the value being displayed.
			 * @param preciseSpeedFontSize Size of font to draw.
			 * @param preciseSpeedFontColour Colour of font drawn.
			 */
			void _drawStandardPreciseBoxForeground(CairoSurface& surface, double preciseValue, unsigned numDecimalPoints,
				string unitString, double preciseSpeedFontSize, colour& preciseSpeedFontColour);

			/**
			 * Draw the standard speedo indicator line.
			 * @param surface Surface to draw to.
			 * @param valueToIndicate Value to indicate on dial gauge.
			 * @param indicatorLineLength Length of speed indicator line.
			 * @param indicatorLineWidth Width of speed indicator line.
			 * @param indicatorLineColour Colour of indicator line.
			 */
			void _drawStandardIndicatorLine(CairoSurface& surface, double valueToIndicate, double indicatorLineLength,
				double indicatorLineWidth, colour& indicatorLineColour);

			/**
			 * Draw a disc like section to indicate the value
			 * @param surface Surface to draw to.
			 * @param valueToIndicate Value to indicate on dial gauge.
			 * @param sectionRadialLength Length of section in radial direction.
			 * @param indicatorRadialSections Array of properties of each section. These _must_ be ordered in the direction
			 *        of the min/max values of the dial.
			 * @param numIndicatorRadialSections Number of sections in array.
			 * @param flashAll If any section is active and has it's flash flag set then all sections flash.
			 */
			void _drawStandardIndicatorSections(CairoSurface& surface, double valueToIndicate, double sectionRadialLength,
				IndicatorRadialSection* indicatorRadialSections, unsigned numIndicatorRadialSections, bool flashAll);

			/** Get dial's radius. */
			double _getRadius();

			/** Get the x coordinate of the centre of the dial.*/
			double _getDialCentreX();

			/** Get the y coordinate of the centre of the dial.*/
			double _getDialCentreY();

			/**
			 * Generate extent string that can be used to get font extendts.
			 * @param value Value to get extent string for.
			 */
			string _generateExtentstring(double value);

		private:

			/** Radius of dial. */
			double _radius;

			/** Centre of dial, x coordinate. */
			double _dialCentreX;

			/** Centre of dial, y coordinate. */
			double _dialCentreY;

			/** startNumber First number shown at the start angle. */
			double _startNumber;

			/** Last number shown at the end angle. */
			double _endNumber;

			/** Numerical distance between marks (including minor marks). ie How much the marked number changes. */
			double _markDistance;

			/** Whether to show minor marks. */
			bool _showMinor;

			/** Whether to show a minor mark prior to the first major indicator mark. */
			bool _showPriorMinor;

			/** Whether to show a minor mark after the last major indicator mark. */
			bool _showLaterMinor;

			/** The font size of the numbers that appear near the marked lines. */
			double _markedFontSize;

			/** Colour of the numbers under the marked lines. */
			colour _markedFontColour;

			/** Number of decimal places to show on marked numbers. */
			unsigned _markedFontDecimalPlaces;

			/** The length of the marked lines. */
			double _lineLength;

			/** Major marked line width. */
			double _majorLineWidth;

			/** Major marked line width. */
			double _minorLineWidth;

			/** Minor marked line width. */
			double _lineStartOffset;

			/** Colour of the major marked lines. */
			colour _majorLineColour;

			/** Colour of the minor marked lines. */
			colour _minorLineColour;

			/** Start angle of the first major mark. Clockwise from x axis in radians. */
			double _startAngle;

			/**
			 * End angle of the last major mark. Clockwise from x axis in radians. If this is less than the
			 * start angle the dial direction is "reversed".
			 */
			double _endAngle;

			/** The length of any flash, if active, in milliseconds. */
			long _sectionsFlashPeriod = 100;

			/** The last time any flashing section was shown. */
			long _sectionsFlashLastShowSec;
			long _sectionsFlashLastShowUSec;
	};
}

#endif