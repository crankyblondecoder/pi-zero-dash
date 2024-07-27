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
			 * Whether the gauge requires a foreground redraw from an instruments latched value.
			 * @param instrument Instrument that latched value.
			 * @returns True if foreground requires drawing.
			 */
			virtual bool _requiresDrawForeground(Instrument* instrument) = 0;

			/**
			 * Add an instrument to this gauge that can be read to get the state to display.
			 * @param instrument Instrument to add. _Not_ owned by this.
			 */
			void _addInstrument(Instrument* instrument);

			/**
			 * Draw the path of standard radiused default box to a cairo context
			 * @param cr Cairo context to draw to.
			 * @param cornerRadius The radius of the box corners.
			 * @param left Coordinate of the left of the box.
			 * @param right Coordinate of the right of the box.
			 * @param top Coordinate of the top of the box.
			 * @param bottom Coordinate of the bottom of the box.
			 */
			void _drawDefaultBoxPath(cairo_t* cr, double cornerRadius, double left, double right, double top, double bottom);

			/**
			 * Draw the path of standard radiused default box to a cairo context
			 * @param cr Cairo context to draw to.
			 * @param topLeftRadius The radius of the top left box corner.
			 * @param topRightRadius The radius of the top right box corner.
			 * @param bottomLeftRadius The radius of the bottom left box corner.
			 * @param bottomRightRadius The radius of the bottom right box corner.
			 * @param left Coordinate of the left of the box.
			 * @param right Coordinate of the right of the box.
			 * @param top Coordinate of the top of the box.
			 * @param bottom Coordinate of the bottom of the box.
			 */
			void _drawBoxPath(cairo_t* cr, double topLeftRadius, double topRightRadius, double bottomLeftRadius,
				double bottomRightRadius, double left, double right, double top, double bottom);

			/**
			 * Draw the standard thermometer symbol path.
			 * @param left Coordinate of the left of the geometry bounding box.
			 * @param right Coordinate of the right of the geometry bounding box.
			 * @param top Coordinate of the top of the geometry bounding box.
			 * @param bottom Coordinate of the bottom of the geometry bounding box.
			 */
			void _drawThermometerPath(cairo_t* cr, double left, double right, double top, double bottom);

			/**
			 * Draw the standard oil can symbol path.
			 * @param cr Cairo context to draw to.
			 * @param left Coordinate of the left of the geometry bounding box.
			 * @param right Coordinate of the right of the geometry bounding box.
			 * @param bottom Coordinate of the bottom of the geometry bounding box.
			 * @param strokeWidth Stroke width that will be used to render the oil can.
			 * @param drawLidHandle If true draw the handle of the oil can lid.
			*/
			void _drawOilCanPath(cairo_t* cr, double left, double right, double bottom, double strokeWidth, bool drawLidHandle);

		private:

			unsigned _instrumentCount;

			Instrument** _instruments;

			/** Whether the first foreground has been drawn for this gauge. */
			bool _firstForegroundDrawn = false;
	};
}

#endif