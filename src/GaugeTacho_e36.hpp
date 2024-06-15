#ifndef PZD_GAUGE_TACHO_E36_H
#define PZD_GAUGE_TACHO_E36_H

#include "GaugeTacho.hpp"

namespace piZeroDash
{
	/**
	 * E36 tacho gauge.
	 */
	class GaugeTacho_e36 : public GaugeTacho
	{
		public:

			virtual ~GaugeTacho_e36();

			/**
			 * @param maxRpm Maximum RPM displayed.
			 * @param redlineRpm RPM value of redline.
			 * @param redLineWarningRpm RPM value of approaching redline warning.
			 * @param globalPositionX X coordinate of position of gauge visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of gauge visual in global coordinates.
			 * @param width Width of gauge visual.
			 * @param height Height of gauge visual.
			 */
			GaugeTacho_e36(unsigned maxRpm, unsigned redlineRpm, unsigned redlineWarningRpm, int globalPositionX,
				int globalPositionY, unsigned width, unsigned height);

			// Impl.
			void _drawBackground(CairoSurface& surface);

			// Impl.
			void _drawForeground(CairoSurface& surface);

		protected:

		private:

			/** Font size for marker numbers. */
			double _markedRpmFontSize;

			/** Font colour for marker numbers. */
			struct colour _markedRpmFontColour = {1.0, 1.0, 1.0, 1.0};

			/** Marker line length. */
			double _lineLength;

			/** Major speed marker line width. */
			double _majorLineWidth;

			/** Colour of the major marked lines. */
			struct colour _majorLineColour = {1.0, 1.0, 1.0, 1.0};

			/** Minor marker line width. */
			double _minorLineWidth;

			/** Colour of the minor marked lines. */
			struct colour _minorLineColour = {1.0, 1.0, 1.0, 0.95};

			/** Amount to move line back towards gauge centre. */
			double _lineStartOffset;

			/** Colour for "normal" pie section of RPM range. */
			struct colour _normalRpmIndictatorColour = {20.0/255.0, 196.0/255.0, 58.0/255.0, 0.95};

			/** Colour for "redline warning" pie section of RPM range. */
			struct colour _redlineWarningRpmIndictatorColour = {227.0/255.0, 186.0/255.0, 75.0/255.0, 0.95};

			/** Colour for "redline" pie section of RPM range. */
			struct colour _redlineRpmIndictatorColour = {245.0/255.0, 15.0/255.0, 15.0/255.0, 0.95};
	};
}

#endif