#ifndef PZD_LATCHER_PICO_H
#define PZD_LATCHER_PICO_H

#include <linux/gpio.h>
#include <stdint.h>

#include "Latcher.hpp"

/** GPIO Pin number for 'Command Active'. This is written. Should be the same as the line number. */
#define PZD_COMMAND_ACTIVE_GPIO 25

/** GPIO Pin number for 'Ready For Command'. This is read. Should be the same as the line number. */
#define PZD_READY_FOR_COMMAND_GPIO 24

/** Number of line event structs in buffer. */
#define GPIO_LINE_EVENT_BUFFER_SIZE 16

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

			/** GPIO Line event read buffer. */
			gpio_v2_line_event _gpioLineEventBuffer[GPIO_LINE_EVENT_BUFFER_SIZE];

			/**
			 * Matches pico latched data indexes to pi zero dash latched data indexes.
			 * @note If -1 is present it indicates an error condition while retrieving the pico latched data index.
			 */
			int _picoLatchedDataIndexes[MAX_LATCHED_INDEXES];

			/**
			 * Set whether the "command active" GPIO line is active.
			 */
			void __setCommandActive(bool masterActive);

			/**
			 * Get the current state of the ready for command active gpio line.
			 * @returns 1 for active, 0 for inactive and -1 for error.
			 */
			int __getReadyForCommandActive();

			/**
			 * Read the available GPIO events, blocking until events are available.
			 * @returns Number of events read into the buffer. -1 For error.
			 */
			int __readGpioEventsBlocking();

			/**
			 * Wait for the 'Ready For Command' GPIO to go active (high).
			 * @note Will block until condition is met or error occurs.
			 */
			void __waitForReadyForCommandActive();

			/**
			 * Wait for the 'Ready For Command' GPIO to go inactive (low).
			 * @note Will block until condition is met or error occurs.
			 */
			void __waitForReadyForCommandInactive();

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
	GET_LATCHED_DATA_INDEX = 0xF1,
	GET_LATCHED_DATA_RESOLUTION = 0xF2,
	GET_LATCHED_DATA = 0xF3
};

/**
 * These are the request ID's
 */
enum SpiCommandRequestId
{
	GET_LATCHED_DATA_INDEX_REQ_ID = 0xE1,
	GET_LATCHED_DATA_RESOLUTION_REQ_ID = 0xE2,
	GET_LATCHED_DATA_REQ_ID = 0xE3
};

#endif