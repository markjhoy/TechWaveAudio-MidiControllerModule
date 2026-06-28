
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H
#include "HardwareI2C.h"

/**
 * Interface to a MCP4725 DAC (single channel, 12 bit DAC with i2c interface)
 */
class Mcp4725 {
public:
    Mcp4725(HardwareI2C *i2c, int8_t address);
    ~Mcp4725() = default;

    /**
     * Writes a value (constrained from 0 to 4095) to the device
     * @param value the value to write
     */
    void write(int value);
private:
    HardwareI2C *_i2c;
    uint8_t _address;
    uint8_t _buffer[2]{};
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H