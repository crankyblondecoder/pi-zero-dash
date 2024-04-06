#ifndef PZD_VISUAL_H
#define PZD_VISUAL_H

#include <cstdint>

#include "adapsurf/Device.hpp"

namespace piZeroDash
{
	/**
	 * Instruments read and interpret data from one or more physical sources.
	 */
	class Visual
	{
		public:

			virtual ~Visual(){};
			Visual(){};

			/**
			 * Bind all visuals to a single drm based adapsurf device.
			 * @note Use adapsurf's cardinfo utility to get the possible values.
			 * @see The adapsurf::DrmDevice constructor for the argument meanings.
			 */
			static bool bindToDrmDevice(unsigned cardNumber, int connectorIndex, int modeIndex);

			static adapsurf::Device* adsDevice;

		protected:

		private:
	};
}

#endif