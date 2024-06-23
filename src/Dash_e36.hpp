#ifndef PZD_DASH_E36_H
#define PZD_DASH_E36_H

#include <cstdint>

#include "Dash.hpp"
#include "GaugeHeadlight_e36.hpp"
#include "GaugeSpeedo_e36.hpp"
#include "GaugeTacho_e36.hpp"
#include "GaugeTurnIndicator_e36.hpp"

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
			 * @param lightHeightPercent Height of any row of lights.
			 */
			Dash_e36(double speedoWidthPercent, double tachoWidthPercent, double turnIndicatorWidthPercent,
				double lightHeightPercent);

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
	};
}

#endif