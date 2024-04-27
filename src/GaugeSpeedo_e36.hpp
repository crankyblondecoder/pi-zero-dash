#ifndef PZD_GAUGE_SPEEDO_E36_H
#define PZD_GAUGE_SPEEDO_E36_H

#include "GaugeSpeedo.hpp"

namespace piZeroDash
{
	/**
	 * E36 speedo gauge.
	 */
	class GaugeSpeedo_e36 : public GaugeSpeedo
	{
		public:

			virtual ~GaugeSpeedo_e36();

			/**
			 * @param maxSpeed Maximum speed displayed by speedo.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeSpeedo_e36(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

			/** Run a test cycle. */
			void test();

		protected:

		private:

			/** Generic speedo. */
			InstrumentSpeedo _speedoInstr;

			/** Maximum speed displayed by the speedo. */
			unsigned _maxSpeed;
	};
}

#endif