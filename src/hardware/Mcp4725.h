//
// Created by Mark J. Hoy on 2026-05-03.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H
#include "HardwareI2C.h"


class Mcp4725 {
public:
    Mcp4725(HardwareI2C *i2c, int8_t address);
    ~Mcp4725() = default;

    void write(int value);
private:
    HardwareI2C *_i2c;
    uint8_t _address;
    uint8_t _buffer[2]{};
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MCP4725_H