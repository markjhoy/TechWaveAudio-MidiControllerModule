
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "SpiDevice.h"

#include "hardware/gpio.h"
#include "hardware/structs/io_bank0.h"

SpiDevice::SpiDevice(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin) {
    _spiBus = spiBus;
    _baudRate = baudRate;
    _clockPin = clockPin;
    _txPin = txPin;
    _rxPin = rxPin;
    _csPin = csPin;

    gpio_put(_csPin, true);

    spi_init(_spiBus, _baudRate);
    spi_set_format( _spiBus, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    gpio_set_function(_clockPin, GPIO_FUNC_SPI);
    gpio_set_function(_txPin, GPIO_FUNC_SPI);
    gpio_set_function(_csPin, GPIO_FUNC_SPI);
}

SpiDevice::~SpiDevice() {
    spi_deinit(_spiBus);
}

void SpiDevice::write(uint8_t *data, uint8_t length) {
    if (_txPin <= 0 || !spi_is_writable(_spiBus)) {
        // write disabled
        return;
    }

    gpio_put(_csPin, false);
    spi_write_blocking(_spiBus, data, length);
    gpio_put(_csPin, true);
}

uint32_t SpiDevice::read(uint8_t *buffer, uint8_t length) {
    if (_rxPin <= 0 || !spi_is_readable(_spiBus)) {
        // read disabled
        return 0;
    }

    gpio_put(_csPin, false);
    uint8_t numRead = spi_read_blocking(_spiBus, 0, buffer, length);
    gpio_put(_csPin, true);
    return numRead;
}
