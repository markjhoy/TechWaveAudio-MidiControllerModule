
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H

#include <cstdint>

#include "SpiDevice.h"
#include "hardware/spi.h"

enum Mcp4902Register {
    Mcp4902_REGISTER_A = 0,
    Mcp4902_REGISTER_B = 1,
};

/**
 * Interface to a MCP4902 DAC (dual channel, 8 bit DAC with spi interface)
 */
class Mcp4902 : public SpiDevice {
public:
    Mcp4902(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin)
        : SpiDevice(spiBus, baudRate, clockPin, txPin, rxPin, csPin) {}

    /**
     * Writes a value out to the MCP4902 to the specific register
     * @param outputRegister the register to write to
     * @param value the value
     */
    void writeValue(Mcp4902Register outputRegister, uint8_t value);
private:
    uint8_t _buffer[2]{};
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4902_H