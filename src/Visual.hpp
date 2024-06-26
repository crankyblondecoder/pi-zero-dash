#ifndef PZD_VISUAL_H
#define PZD_VISUAL_H

#include <cstdint>

#include "adapsurf/CairoSurface.hpp"
#include "adapsurf/DrmDevice.hpp"

#define M_PI 3.14159265358979323846

using namespace adapsurf;

namespace piZeroDash
{
	/**
	 * Basic colour.
	 */
	struct colour
	{
		/** Red */
		double r;
		/** Green */
		double g;
		/** Blue */
		double b;
		/** Alpha */
		double a;
	};

	/**
	 * Rectangular (box) bounds.
	 */
	struct bounds
	{
		/** Coordinate of left side of box. */
		double left;
		/** Coordinate of right side of box. */
		double right;
		/** Coordinate of top side of box. */
		double top;
		/** Coordinate of bottom side of box. */
		double bottom;
	};

	/**
	 * Instruments read and interpret data from one or more physical sources.
	 */
	class Visual
	{
		public:

			virtual ~Visual();

			/**
			 * @param globalPositionX X coordinate of position of visual in global coordinates.
			 * @param globalPositionY Y coordinate of position of visual in global coordinates.
			 * @param width Width of visual.
			 * @param height Height of visual.
			 * @param hasForeground If true then generate foreground visual.
			 * @param hasBackground If true then generate background visual.
			 */
			Visual(int globalPositionX, int globalPositionY, unsigned width, unsigned height, bool hasForeground,
				bool hasBackground);

			/**
			 * Bind all visuals to a single drm based adapsurf device.
			 * @note Use adapsurf's cardinfo utility to get the possible values.
			 * @see The adapsurf::DrmDevice constructor for the argument meanings.
			 */
			static bool bindToDrmDevice(unsigned cardNumber, int connectorIndex, int modeIndex);

			/**
			 * Set the clear colour of the root surface.
			 */
			static void setRootClearColour(double red, double green, double blue);

			static DrmDevice* adsDevice;

			/**
			 * Draw the background of this visual.
			 */
			void drawBackground();

			/**
			 * Draw the foreground of this visual.
			 */
			void drawForeground();

		protected:

			/** Get width of this visual in pixels. */
			unsigned _getWidth();

			/** Get height of this visual in pixels. */
			unsigned _getHeight();

			/**
			 * Draw the background of this visual.
			 */
			void _drawBackground();

			/**
			 * Draw to the given background surface.
			 * @param surface Surface to draw to.
			 */
			virtual void _drawBackground(CairoSurface& surface) = 0;

			/**
			 * Compose the given visuals background _onto_ this visuals background.
			 */
			void _composeBackground(Visual& visual);

			/**
			 * Draw the foreground of this visual.
			 */
			void _drawForeground();

			/**
			 * Draw to the given background surface.
			 * @param surface Surface to draw to.
			 */
			virtual void _drawForeground(CairoSurface& surface) = 0;

			/**
			 * Compose the given visuals foreground _onto_ this visuals foreground.
			 */
			void _composeForeground(Visual& visual);

			/**
			 * Compose this visuals background to the back buffer of the display.
			 */
			void _composeBackgroundToDisplay();

			/**
			 * Compose this visuals foreground to the back buffer of the display.
			 */
			void _composeForegroundToDisplay();

			/**
			 * Commit this visual to be displayed. ie Page flip the backbuffer into the front buffer.
			*/
			void _commitToDisplay();

			/** Get the x coordinate of the global position. */
			double _getGlobalPositionX();

			/** Get the y coordinate of the global position. */
			double _getGlobalPositionY();

		private:

			/** Background surface. */
			CairoSurface* _backgroundSurface;

			/** Foreground surface. */
			CairoSurface* _foregroundSurface;

			/** Width of visual in pixels. */
			unsigned _width;

			/** Height of visual in pixels. */
			unsigned _height;

			/** Global position, x coordinate. */
			double _globalPosnX;

			/** Global position, y coordinate. */
			double _globalPosnY;
	};
}

#endif