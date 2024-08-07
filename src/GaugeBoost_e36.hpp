#ifndef PZD_GAUGE_BOOST_E36_H
#define PZD_GAUGE_BOOST_E36_H

#include "GaugeBoost.hpp"

namespace piZeroDash
{
	/**
	 * E36 boost gauge.
	 */
	class GaugeBoost_e36 : public GaugeBoost
	{
		public:

			virtual ~GaugeBoost_e36();

			/**
			 * @param minBoost Minimum boost displayed.
			 * @param maxBoost Minimum boost displayed.
			 * @param neutralBoost Neutral boost value.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeBoost_e36(int minBoost, int maxBoost, int neutralBoost, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

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

			/** Colour for below neutral pie section of range. */
			colour _belowNeutralIndictatorColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.95};

			/** Colour for above neutral pie section of range. */
			colour _aboveNeutralIndictatorColour = {20.0/255.0, 196.0/255.0, 58.0/255.0, 0.95};

			/** Indicator line colour if indicator section mode isn't being used. */
			colour _indiactorLineColour = {21.0/255.0, 54.0/255.0, 214.0/255.0, 0.85};
	};
}

#endif