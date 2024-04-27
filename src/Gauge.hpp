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

		protected:

			/**
			 * Add an instrument to this gauge that can be read to get the state to display.
			 * @param instrument Instrument to add. Not owned by this.
			 */
			void _addInstrument(Instrument* instrument);

		private:

			unsigned _instrumentCount;

			Instrument** _instruments;
	};
}

#endif