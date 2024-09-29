#ifndef PZD_LATCHER_PICO_H
#define PZD_LATCHER_PICO_H

#include <stdint.h>

#include "Latcher.hpp"

/** GPIO Pin number. Should be the same as the line number. */
#define PZD_MASTER_ACTIVE_GPIO 25

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

			/** GPIO device (chip) file descriptor. */
			int _gpioChipFd = -2;

			/** GPIO line file descriptor. */
			__s32 _gpioLineFd = -1;

			/** Whether this latcher is ready. */
			bool _ready = false;

			/**
			 * Set whether the "master active" GPIO line is active.
			 */
			void __setMasterActive(bool masterActive);

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