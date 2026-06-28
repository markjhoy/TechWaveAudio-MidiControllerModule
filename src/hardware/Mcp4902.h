
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H

#include <cstdint>
#include "hardware/spi.h"

/**
 * Interface to a MCP4902 DAC (dual channel, 8 bit DAC with spi interface)
 */
class Mcp4902 {
public:
    Mcp4902(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);
    ~Mcp4902();

    /**
     * Writes byte data out on the SPI interface to the device
     * @param data pointer to the data to write
     * @param size number of bytes to write
     */
    void write(uint8_t *data, uint8_t size);

    /**
     * Reads byte data from the SPI interface
     * @param buffer the buffer to store the read data
     * @param maxSize the maximum size of the buffer
     * @return the total number of bytes read
     */
    uint8_t read(uint8_t *buffer, uint8_t maxSize);
private:
    spi_inst_t *_spiBus;
    int _baudRate;
    int _clockPin;
    int _txPin;
    int _rxPin;
    int _csPin;
    bool _hasBeenInitialized = false;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H