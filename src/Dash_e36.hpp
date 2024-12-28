#ifndef PZD_DASH_E36_H
#define PZD_DASH_E36_H

#include <cstdint>

#include "Dash.hpp"
#include "GaugeBoost_e36.hpp"
#include "GaugeDoorOpen_e36.hpp"
#include "GaugeEcuWarning_e36.hpp"
#include "GaugeEngineTemp_e36.hpp"
#include "GaugeFuelLevel_e36.hpp"
#include "GaugeHeadlight_e36.hpp"
#include "GaugeLowOilPressure_e36.hpp"
#include "GaugeLowVoltage_e36.hpp"
#include "GaugeOilTemperature_e36.hpp"
#include "GaugeParkBrake_e36.hpp"
#include "GaugeSpeedo_e36.hpp"
#include "GaugeTacho_e36.hpp"
#include "GaugeTurnIndicator_e36.hpp"
#include "LatcherPico.hpp"

namespace piZeroDash
{
	/**
	 * A dash displays one or more gauges.
	 */
	class Dash_e36 : public Dash
	{
		public:

			virtual ~Dash_e36();

			/**
			 * @param speedoWidthPercent Speedo width as a percentage of the width of the dash.
			 * @param tachoWidthPercent Tacho width as a percentage of the width of the dash.
			 * @param turnIndicatorWidthPercent Turn indicator width as a percentage of the width of the dash.
			 * @param lightHeightPercent Height of any row of lights, as percentage of the dash height.
			 * @param engineTempWidthPercent Width of engine temp box, as percentage of dash width.
			 * @param boostWidthPercent Boost gauge width as percentage of width of dash.
			 */
			Dash_e36(double speedoWidthPercent, double tachoWidthPercent, double turnIndicatorWidthPercent,
				double lightHeightPercent, double engineTempWidthPercent, double boostWidthPercent);

			/** Set the latcher that communicates with the Pi Pico. */
			void setPicoLatcher(LatcherPico* picoLatcher);

			/** Set the latcher that communicates with the Pi Zero GPIO connected instruments. */
			void setPiZeroLatcher(Latcher* latcher);

		protected:

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

		private:

			/** Speedo gauge. */
			GaugeSpeedo_e36* _speedo;

			/** Tacho gauge. */
			GaugeTacho_e36* _tacho;

			/** Left turn indicator. */
			GaugeTurnIndicator_e36* _turnLeft;

			/** Right turn indicator. */
			GaugeTurnIndicator_e36* _turnRight;

			/** Headlight off/low/high beam light. */
			GaugeHeadlight_e36* _headlight;

			/** Engine temperature. */
			GaugeEngineTemp_e36* _engineTemp;

			/** Forced induction boost. */
			GaugeBoost_e36* _boost;

			/** Fuel level gauge. */
			GaugeFuelLevel_e36* _fuelLevel;

			/** Park brake gauge. */
			GaugeParkBrake_e36* _parkBrake;

			/** Door open gauge. */
			GaugeDoorOpen_e36* _doorOpen;

			/** Low voltage gauge. */
			GaugeLowVoltage_e36* _lowVoltage;

			/** Low oil pressure gauge. */
			GaugeLowOilPressure_e36* _lowOilPressure;

			/** ECU Warning gauge. */
			GaugeEcuWarning_e36* _ecuWarning;

			/** Engine oil temperature. */
			GaugeOilTemperature_e36* _oilTemp;
	};
}

#endif