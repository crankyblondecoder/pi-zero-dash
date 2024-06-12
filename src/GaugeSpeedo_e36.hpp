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

		protected:

		private:

			/** Font size for speed markers. */
			double _markedSpeedFontSize;

			/** Font colour for speed markers. */
			struct colour _markedSpeedFontColour = {1.0, 1.0, 1.0, 1.0};

			/** Speed marker line length. */
			double _lineLength;

			/** Major speed marker line width. */
			double _majorLineWidth;

			/** Colour of the major marked lines. */
			struct colour _majorLineColour = {1.0, 1.0, 1.0, 1.0};

			/** Minor speed marker line width. */
			double _minorLineWidth;

			/** Colour of the minor marked lines. */
			struct colour _minorLineColour = {1.0, 1.0, 1.0, 0.95};

			/** Amount to move line back towards gauge centre. */
			double _lineStartOffset;

			/**
			 * Colour of the precise speed background.
			 * Blue(ish): 21, 54, 214
			 */
			struct colour _preciseSpeedBackgroundColour = {21.0/255.0, 54.0/255.0, 214.0/255.0, 0.85};

			/** Width of precise speed background box. */
			double _preciseSpeedBackgroundWidth;

			/** Height of precise speed background box. */
			double _preciseSpeedBackgroundHeight;
	};
}

#endif