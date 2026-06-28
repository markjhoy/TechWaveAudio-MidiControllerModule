
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


class Mcp4902 {
public:
    Mcp4902(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);
    ~Mcp4902();

    void write(uint8_t *data, uint8_t size);
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