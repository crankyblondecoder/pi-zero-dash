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
			 * @param lineLength Marker line length. The line that marks the speed.
			 * @param majorLineWidth Marker line width for major graduations.
			 * @param minorLineWidth Marker line width for the minor graduations.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeSpeedo_e36(unsigned maxSpeed, double lineLength, double majorLineWidth, double minorLineWidth,
				int globalPositionX, int globalPositionY, unsigned width, unsigned height);

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

			/** Generic speedo. */
			InstrumentSpeedo _speedoInstr;

			/** Speed marker line length. */
			double _lineLength;

			/** Major speed marker line width. */
			double _majorLineWidth;

			/** Minor speed marker line width. */
			double _minorLineWidth;
	};
}

#endif