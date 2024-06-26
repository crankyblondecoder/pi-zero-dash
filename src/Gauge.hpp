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
			 * Draw the path of standard radiused default box to a cairo context
			 * @param cr Cairo context to draw to.
			 * @param cornerRadius The radius of the box corners.
			 * @param left Coordinate of the left of the box.
			 * @param right Coordinate of the right of the box.
			 * @param top Coordinate of the top of the box.
			 * @param bottom Coordinate of the bottom of the box.
			 */
			void _drawDefaultBoxPath(cairo_t* cr, double cornerRadius, double left, double right, double top, double bottom);

		private:

			unsigned _instrumentCount;

			Instrument** _instruments;
	};
}

#endif