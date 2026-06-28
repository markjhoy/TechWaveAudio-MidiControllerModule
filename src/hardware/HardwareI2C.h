
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_HARDWAREI2C_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_HARDWAREI2C_H
#include <cstdio>
#include <vector>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/structs/io_bank0.h"
#include "pico/stdio.h"
#include "pico/binary_info/code.h"


class HardwareI2C {
public:
    HardwareI2C(i2c_inst_t *i2c, int sdaPin, int sclPin, long baudRate);
    ~HardwareI2C() = default;

    void write(uint8_t address, uint8_t *data, uint32_t length) const;
    int read(uint8_t address, uint8_t *data, int length) const;
    [[nodiscard]] std::vector<uint8_t> scanBus() const;
private:
    i2c_inst_t *_i2c;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_HARDWAREI2C_H