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

/** The size of the tx and rx buffers. */
#define TX_RX_BUFFER_SIZE 16

/** The Pico latched data command/response frame size, in bytes. */
#define PICO_SPI_LATCHED_DATA_CMD_RESP_FRAME_SIZE 8

/** Wait for ready timeout. Applies to both Active/Inactive tests. */
#define WAIT_FOR_READY_TIMEOUT 2

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

			// Impl
			double getLatchedDataValueDouble(LatchedDataIndex dataIndex);

			// Impl.
			bool getLatchedDataValueBool(LatchedDataIndex dataIndex);

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
			uint8_t _txBuf[TX_RX_BUFFER_SIZE];

			/** Common SPI receive buffer. */
			uint8_t _rxBuf[TX_RX_BUFFER_SIZE];

			/** GPIO Line event read buffer. */
			gpio_v2_line_event _gpioLineEventBuffer[GPIO_LINE_EVENT_BUFFER_SIZE];

			/**
			 * Matches pico latched data indexes to pi zero dash latched data indexes.
			 * @note latched data indexes are never 0, which indicates "not assigned".
			 * @note If -1 is present it indicates an error condition while retrieving the pico latched data index.
			 */
			int _picoLatchedDataIndexes[MAX_LATCHED_INDEXES];

			/**
			 * Pico latched data resolutions that match the latched data indexes, array indexes match between these two arrays.
			 */
			int _picoLatchedDataResolutions[MAX_LATCHED_INDEXES];

			/**
			 * Pico raw latched data. ie Hasn't had resolutions applied yet.
			 */
			int _picoLatchedDataRaw[MAX_LATCHED_INDEXES];

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
			 * Read the available GPIO events, not blocking until events are available.
			 * @param timeout Maximum number of milliseconds to wait for input for.
			 * @returns Number of events read into the buffer. -1 For error.
			 */
			int __readGpioEventsNonBlocking(int timeout);

			/**
			 * Clear all GPIO events from the event buffer.
			 */
			void __clearGpioEvents();

			/**
			 * Wait for the 'Ready For Command' GPIO to go active (high).
			 * @note Will block until condition is met or error occurs up to an abitrary timeout.
			 * @returns True if ready for command is active, false if an error condition or timeout occurred, or ready for
			 *          command is currently inactive..
			 */
			bool __waitForReadyForCommandActive();

			/**
			 * Wait for the 'Ready For Command' GPIO to go inactive (low).
			 * @note Will block until condition is met or error occurs up to an abitrary timeout.
			 * @returns True if ready for command is inactive, false if an error condition or timeout occurred, or ready for
			 *          command is currently active..
			 */
			bool __waitForReadyForCommandInactive();

			/** Clear the tx buffer with 0's */
			void __clearTxBuffer();

			/**
			 * Transmit/recieve SPI data from Pico.
			 * @note SPI transmits and recieves the same amount of data every SPI frame.
			 * @note The Pico will return a request ID of 0xFF when a command error occurs.
			 * @param txBuf Transmit buffer.
			 * @param rxBuf Recieve buffer
			 * @param length Length of buffers and amount of data to read from SPI bus.
			 * @returns True for success. False otherwise.
			 */
			bool __picoSpiTxRx(uint8_t* txBuf, uint8_t* rxBuf, int length);

			/**
			 * Send a command to the Pico, then wait for reply.
			 * Assumes command is in tx buffer and reply is in rx buffer.
			 * @note The first entry in the tx buffer is assumed to be the command ID.
			 * @returns True if command was successful. False otherwise.
			 */
			bool __sendRecvCommand();

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
			 * Download a single latched data resolution for a previously retrieved latched data index.
			 * @param latchedDataIndex Latched data index of resolution to download.
			 * @returns Resolution supplied from the Pico. -1 if unsuccessful.
			 */
			int __downloadLatchedDataResolution(int latchedDataIndex);

			/**
			 * Download all the resolutions for the previously retrieved latched data indexes.
			 */
			void __downloadLatchedDataResolutions();

			/**
			 * Download all the raw latched data for the previously retrieved latched data indexes.
			 */
			void __downloadLatchedDataRaw();

			/** Get the resolved latched data float value for the given index. */
			double __getLatchedDoubleValue(int latchedDataIndex);

			/** Get the resolved latched data bool value for the given index. */
			bool __getLatchedBoolValue(int latchedDataIndex);
	};
}

/**
 * Commands to get latched data from the pico via SPI.
 * @note These have been copied from pico_dash_spi.h
 */
enum PicoLatchedDataSpiCommand
{
	GET_LATCHED_DATA_INDEX = 0xF1,
	GET_LATCHED_DATA_RESOLUTION = 0xF2,
	GET_LATCHED_DATA = 0xF3
};

#endif