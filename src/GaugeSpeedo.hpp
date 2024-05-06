#ifndef PZD_GAUGE_SPEEDO_H
#define PZD_GAUGE_SPEEDO_H

#include "Gauge.hpp"
#include "InstrumentSpeedo.hpp"

namespace piZeroDash
{
	/**
	 * Generic speedo gauge.
	 */
	class GaugeSpeedo : public Gauge
	{
		public:

			virtual ~GaugeSpeedo();

			/**
			 * @param maxSpeed Maximum speed displayed by speedo.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeSpeedo(unsigned maxSpeed, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void test();

		protected:

			/** Get the maximum speed displayed. */
			unsigned _getMaxSpeed();

		private:

			/** Generic speedo. */
			InstrumentSpeedo _speedoInstr;

			/** Maximum speed displayed by the speedo. */
			unsigned _maxSpeed;
	};
}

#endif