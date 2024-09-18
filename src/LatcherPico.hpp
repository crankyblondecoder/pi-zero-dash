#ifndef PZD_LATCHER_PICO_H
#define PZD_LATCHER_PICO_H

#include <stdint.h>

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

			// Impl.
			bool _isReady();

		private:

			/** SPI device file descriptor. */
			int _spiFd = -2;

			/** Whether this latcher is ready. */
			bool _ready = false;

			/**
			 * Transmit/recieve SPI data from pico.
			 * @note SPI transmits and recieves the same amount of data every frame.
			 * @param rxBuf Recieve buffer
			 * @param txBuf Transmit buffer.
			 * @param length Length of buffers
			 */
			void __picoSpiTxRx(uint8_t* rxBuf, uint8_t* txBuf, int length);
	};
}

#endif