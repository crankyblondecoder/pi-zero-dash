#ifndef PZD_GAUGE_FUEL_LEVEL_E36_H
#define PZD_GAUGE_FUEL_LEVEL_E36_H

#include "GaugeFuelLevel.hpp"

namespace piZeroDash
{
	/**
	 * E36 fuel level gauge.
	 */
	class GaugeFuelLevel_e36 : public GaugeFuelLevel
	{
		public:

			virtual ~GaugeFuelLevel_e36();

			/**
			 * @param maxFuelLevel Maximum fuel level displayed.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeFuelLevel_e36(int maxFuelLevel, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

		protected:

		private:

			/** Marker line length. */
			double _lineLength;

			/** Font colour for marker numbers. */
			colour _markedFontColour = {1.0, 1.0, 1.0, 1.0};

			/** Colour of the major marked lines. */
			colour _majorLineColour = {1.0, 1.0, 1.0, 1.0};

			/** Major marker line width. */
			double _majorLineWidth;

			/** Colour of the minor marked lines. */
			colour _minorLineColour = {1.0, 1.0, 1.0, 0.95};

			/** Indicator line colour if indicator section mode isn't being used. */
			colour _indiactorLineColour = {21.0/255.0, 54.0/255.0, 214.0/255.0, 0.85};

			/** Low fuel level indicator colour. */
			colour _lowFuelLevelColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.95};

			/** Below zero fuel level indicator colour. */
			colour _belowZeroFuelLevelColour = {245.0/255.0, 15.0/255.0, 15.0/255.0, 0.95};
	};
}

#endif