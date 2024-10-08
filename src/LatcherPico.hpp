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

			/** Common SPI transmit buffer. */
			uint8_t _txBuf[5];

			/** Common SPI receive buffer. */
			uint8_t _rxBuf[5];

			/**
			 * Matches pico latched data indexes to pi zero dash latched data indexes.
			 * @note If -1 is present it indicates an error condition while retrieving the pico latched data index.
			 */
			int _picoLatchedDataIndexes[MAX_LATCHED_INDEXES];

			/**
			 * Set whether the "master active" GPIO line is active.
			 */
			void __setMasterActive(bool masterActive);

			/**
			 * Transmit/recieve SPI data from pico.
			 * @note SPI transmits and recieves the same amount of data every frame.
			 * @param txBuf Transmit buffer.
			 * @param rxBuf Recieve buffer
			 * @param length Length of buffers
			 * @param reqId This request id indicates the start of the receive stream. Keep on transferring until this value
			 *        is recieved and the total amount of subsequent data recieved is length - 1. Use 0 for null (ie 0 is
			 *        not a valid request id).
			 * @returns True for success. False otherwise.
			 */
			bool __picoSpiTxRx(uint8_t* txBuf, uint8_t* rxBuf, int length, uint8_t reqId);

			/**
			 * Download a single latched data index from the pico.
			 * @param latchedDataIndexName String name of latched data index. _Must_ be 3 chars long.
			 * @returns Latched data index supplied from the Pico. -1 if could not get index for any reason.
			 */
			int __downloadLatchedDataIndex(const char* latchedDataIndexName);

			/**
			 * Download all of the latched data indexes from the pico that are required to retrieve latched data.
			 */
			void __downloadLatchedDataIndexes();

			/**
			 * Download all the resolutions for the previously retrieved latched data indexes.
			 */
			void __downloadLatchedDataResolutions();
	};
}

/**
 * Commands to get data from the pico via SPI.
 * @note These have been copied from pico_dash_spi.h
 */
enum SpiCommand
{
	GET_LATCHED_DATA_INDEX = 0x11,
	GET_LATCHED_DATA_RESOLUTION = 0x12,
	GET_LATCHED_DATA = 0x13
};


#endif