
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_SPIDEVICE_H
#define TECHWAVEAUDIO_MCM_SPIDEVICE_H
#include <cstdint>

#include "hardware/spi.h"
#include "pico/sem.h"

/**
 * Base class for handling an SPI device
 */
class SpiDevice {
public:
    /**
     * Creates a SPI device object
     * @param spiBus the SPI bus for data transmission
     * @param baudRate (max up to 50 MHz theoretical limit)
     * @param clockPin the SPI clock pin
     * @param txPin the SPI write pin
     * @param rxPin the SPI read pin (not used)
     * @param csPin the chip select enable pin
     */
    SpiDevice(spi_inst_t *spiBus, int baudRate, int dataBits, int clockPin, int txPin, int rxPin, int csPin, int clockMode = 0);

    virtual ~SpiDevice();

protected:
    /**
     * Writes a value out on the SPI interface to the device
     * @param data buffer to the data to write
     * @param length the length of the buffer
     */
    void write(void *data, uint8_t length);

    /**
     * Reads data in from the device via the SPI interface
     * @param buffer the buffer to write data to
     * @param length the max length of the buffer
     * @return the number of bytes read
     */
    uint32_t read(void *buffer, uint8_t length);

private:
    spi_inst_t *_spiBus;
    int _dataBits;
    int _baudRate;
    int _clockPin;
    int _txPin;
    int _rxPin;
    int _csPin;
};


#endif //TECHWAVEAUDIO_MCM_SPIDEVICE_H