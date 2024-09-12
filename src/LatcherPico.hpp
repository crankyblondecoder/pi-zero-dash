#ifndef PZD_LATCHER_PICO_H
#define PZD_LATCHER_PICO_H

#include "Latcher.hpp"

namespace piZeroDash
{
	/**
	 * Latched data sourced from Pi Pico.
	 */
	class LatcherPico : public Latcher
	{
		public:

			virtual ~LatcherPico();
			LatcherPico();

		protected:

			// Impl.
			void _poll();

		private:

			/** SPI device file descriptor. */
			int _spiFd = -2;
	};
}

#endif