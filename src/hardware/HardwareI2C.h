
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
#include "pico/sem.h"
#include "pico/binary_info/code.h"


/**
 * Interface to a hardware I2C communications bus
 */
class HardwareI2C {
public:
    HardwareI2C(i2c_inst_t *i2c, int sdaPin, int sclPin, long baudRate);
    ~HardwareI2C() = default;

    /**
     * Writes data out on the i2c bus
     * @param address the i2c address
     * @param data pointer to the data to write
     * @param length total number of bytes to write
     */
    void write(uint8_t address, uint8_t *data, uint32_t length);

    /**
     * Reads data from the i2c bus
     * @param address the i2c address
     * @param data pointer to the data for the read buffer
     * @param length max length of the read buffer
     * @return total number of bytes read
     */
    int read(uint8_t address, uint8_t *data, int length);

    /**
     * Scans the i2c bus for valid addresses and devices
     * @return A vector containing valid addresses found on the i2c bus
     */
    [[nodiscard]] std::vector<uint8_t> scanBus();
private:
    i2c_inst_t *_i2c;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_HARDWAREI2C_H