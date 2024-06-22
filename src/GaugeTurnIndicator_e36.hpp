#ifndef PZD_GAUGE_TURN_INDICATOR_E36_H
#define PZD_GAUGE_TURN_INDICATOR_E36_H

#include "GaugeTurnIndicator.hpp"

namespace piZeroDash
{
	/**
	 * E36 turn indicator gauge.
	 */
	class GaugeTurnIndicator_e36 : public GaugeTurnIndicator
	{
		public:

			virtual ~GaugeTurnIndicator_e36();

			/**
			 * @param left Show a left turn indicator. Default is right.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTurnIndicator_e36(bool left, int globalPositionX, int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

		protected:

		private:

			/** Arrow background colour. */
			struct colour _backgroundColour = {0.5, 0.5, 0.5, 0.5};

			/** Arrow foreground colour. */
			// green: 20, 227, 48
			struct colour _foregroundColour = {20.0/255.0, 227.0/255, 48.0/255.0, 1.0};
	};
}

#endif