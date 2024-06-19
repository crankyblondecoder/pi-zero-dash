#ifndef PZD_GAUGE_BAR_H
#define PZD_GAUGE_BAR_H

#include <cstdint>
#include <string>

#include "Gauge.hpp"

namespace piZeroDash
{
	/**
	 * Generic bar gauge.
	 */
	class GaugeBar : public Gauge
	{
		public:

			virtual ~GaugeBar();

			/**
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeBar(int globalPositionX, int globalPositionY, unsigned width, unsigned height);

		protected:

			/**
			 * Draw a default bar gauge background.
			 * @param surface Surface to draw to.
			 */
			void _drawDefaultBackground(CairoSurface& surface);

			/**
			 * Draw a default turn indicator foreground.
			 * @param surface Surface to draw to.
			 */
			void _drawDefaultForeground(CairoSurface& surface);

		private:

			/** Angle from horizontal. Uses cairo coordinate system. */
			double _angle = -M_PI / 2.0;

			/** Corner radius of the outer box. */
			double _cornerRadius = 5;

			/** Number of sections the bar is split into. */
			double _numSections;

			/** Array of colours that correlate to the bar sections. */
			colour* _sectionColours;

			/** The height of the start label. */
			double _startLabelHeight = 0.0;

			/** The height of the end label. */
			double _endLabelHeight = 0.0;

			/** The label text at the start of the bar */
			std::string _startLabelText = "";

			/** The label text at the end of the bar */
			std::string _endLabelText = "";

			/** The label font size. */
			double _labelFontSize = 16.0;
	};
}

#endif