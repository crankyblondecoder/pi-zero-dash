#ifndef PZD_GAUGE_OIL_TEMP_E36_H
#define PZD_GAUGE_OIL_TEMP_E36_H

#include "GaugeStdLight.hpp"
#include "InstrumentOilTemperature.hpp"

namespace piZeroDash
{
	/**
	 * E36 oil temperature gauge.
	 */
	class GaugeOilTemperature_e36 : public GaugeStdLight
	{
		public:

			virtual ~GaugeOilTemperature_e36();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeOilTemperature_e36(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

			// Impl.
			bool inTestMode();

		protected:

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

			// Impl.
			bool _requiresDrawForeground(Instrument* instrument);

		private:

			/** Reads current oil temperature. Assume degrees celcius. */
			InstrumentOilTemperature _oilTempInstr;

			/** Last oil temperature that foreground was drawn for. */
			double _lastOilTemp = 1 << 30;

			/** Temperature below which cold oil temp is indicated.*/
			double _lowTempThreshold = 70.0;

			/** Temperature above which hot oil temp is indicated.*/
			double _highTempThreshold = 120.0;

			/** Default background colour. Basically this is light off. */
			struct colour _backgroundColour = {0.5, 0.5, 0.5, 0.5};

			/** Oil temperature too cold outline colour. */
			struct colour _tempColdStrokeColour = {1.0, 1.0, 1.0, 0.85};

			/** Oil temperature too cold background colour. */
			struct colour _tempColdBackgroundColour = {21.0/255.0, 54.0/255.0, 214.0/255.0, 0.85};

			/** Oil temperature okay outline colour. */
			struct colour _tempOkayStrokeColour = {20.0/255.0, 227.0/255, 48.0/255.0, 0.85};

			/** Oil temperature too high outline colour. */
			struct colour _tempHighStrokeColour = {1.0, 0.0, 0.0, 1.0};

			/** Oil temperature too high background colour. */
			struct colour _tempHighBackgroundColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.9};
	};
}

#endif