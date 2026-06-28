
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "HardwareI2C.h"

#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/structs/io_bank0.h"

HardwareI2C::HardwareI2C(i2c_inst_t *i2c, int sdaPin, int sclPin, long baudRate) {
    _i2c = i2c;
    i2c_init(i2c, baudRate);
    gpio_set_function(sdaPin, GPIO_FUNC_I2C);
    gpio_set_function(sclPin, GPIO_FUNC_I2C);
}

void HardwareI2C::write(uint8_t address, uint8_t *data, uint32_t length) const {
    switch (i2c_write_blocking(_i2c, address, data, length, true)) {
        case PICO_ERROR_GENERIC:
            printf("[%d] addr not acknowledged!\n", address);
            break;
        case PICO_ERROR_TIMEOUT:
            printf("I2C timeout, address [%d]]!\n", address);
            break;
        default:
            break;
    }
}

int HardwareI2C::read(uint8_t address, uint8_t *data, int length) const {
    return i2c_read_blocking(_i2c, address, data, length, false);
}

std::vector<uint8_t> HardwareI2C::scanBus() const {
    std::vector<uint8_t> result;

    for (int addr = 0; addr < (1 << 7); ++addr) {
        if ((addr & 0x78) == 0 || (addr & 0x78) == 0x78) {
            continue;
        }

        int ret;
        uint8_t rxdata;
        ret = i2c_read_blocking(_i2c, addr, &rxdata, 1, false);
        if (ret != PICO_ERROR_GENERIC) {
            result.push_back(addr);
        }
    }

    return result;
}
