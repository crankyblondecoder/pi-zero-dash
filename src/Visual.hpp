#ifndef PZD_VISUAL_H
#define PZD_VISUAL_H

#include <cstdint>

#include "adapsurf/CairoSurface.hpp"
#include "adapsurf/Device.hpp"

using namespace adapsurf;

namespace piZeroDash
{
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

			static Device* adsDevice;

		protected:

			/**
			 * Draw to the given background surface.
			 * @param surface Surface to draw to.
			 */
			virtual void _drawBackground(CairoSurface& surface) = 0;

			/**
			 * Draw to the given background surface.
			 * @param surface Surface to draw to.
			 */
			virtual void _drawForeground(CairoSurface& surface) = 0;

		private:

			/** Background surface. */
			CairoSurface* _backgroundSurface;

			/** Foreground surface. */
			CairoSurface* _foregroundSurface;
	};
}

#endif