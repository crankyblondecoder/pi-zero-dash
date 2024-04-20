#ifndef PZD_DASH_H
#define PZD_DASH_H

#include <cstdint>

#include "Gauge.hpp"
#include "Visual.hpp"

#define NUM_GAUGES 64

namespace piZeroDash
{
	/**
	 * A dash displays one or more gauges.
	 */
	class Dash : public Visual
	{
		public:

			virtual ~Dash(){};

			/**
			 * Create a dash that takes up the entire size of the display.
			 */
			Dash();

			/**
			 * Strobe all gauges and update dash.
			 * @param numberOfStrobes Number of strobes to perform. Use 0 for continuously strobing until told to stop.
			 */
			void strobe(unsigned numberOfStrobes);

			/** Stop any pending strobing. */
			void stopStrobing();

		protected:

			/**
			 * Add a gauge to this dash.
			 * @param gauge Pointer to gauge to add. Owned by this.
			 */
			void _addGauge(Gauge* gauge);

		private:

			/** Array of pointers to gauges this dash contains. */
			Gauge** _gauges;

			/** Number gauge pointers in array. */
			unsigned _gaugeCount;

			/** If true then the background for this dash and all gauge backgrounds has been generated. */
			bool _backgroundGenerated;

			/** Generate the background for the entire dash. Which includes the composition of all gauge backgrounds. */
			void __generatedBackground();
	};
}

#endif