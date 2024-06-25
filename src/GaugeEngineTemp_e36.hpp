#ifndef PZD_GAUGE_ENGINE_TEMP_E36_H
#define PZD_GAUGE_ENGINE_TEMP_E36_H

#include "Gauge.hpp"
#include "InstrumentEngineTemp.hpp"

namespace piZeroDash
{
	/**
	 * E36 headlight gauge.
	 */
	class GaugeEngineTemp_e36 : public Gauge
	{
		public:

			virtual ~GaugeEngineTemp_e36();

			/**
			 * @param minTemp Minimum temperature.
			 * @param maxTemp Maximum temperature.
			 * @param coldLimitTemp Cold to normal threshold temperature.
			 * @param normalLimitTemp Normal to hot threshold temperature.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeEngineTemp_e36(int minTemp, int maxTemp, int coldLimitTemp, int normalLimitTemp, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

		private:

			/** Minimum temperature. */
			int _minTemp;

			/** Maximum temperature. */
			int _maxTemp;

			/** Cold to normal threshold. */
			int _coldLimitTemp;

			/** Normal to hot threshold. */
			int _normalLimitTemp;

			/** Engine temperature instrument. */
			InstrumentEngineTemp _engineTempInstr;

			/** Box colour - cold. */
			struct colour _boxColourCold = {21.0/255.0, 54.0/255.0, 214.0/255.0, 0.75};

			/** Box colour - normal. */
			struct colour _boxColourNormal = {20.0/255.0, 227.0/255, 48.0/255.0, 0.75};

			/** Box colour - hot. */
			struct colour _boxColourHot = {1.0, 0.0, 0.0, 1.0};

			/** Font colour. */
			struct colour _fontColour = {1.0, 1.0, 1.0, 1.0};
	};
}

#endif