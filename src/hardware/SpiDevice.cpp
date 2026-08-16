
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "SpiDevice.h"

#include "hardware/gpio.h"
#include "hardware/structs/io_bank0.h"

#define SPI_LOOP_COUNT_SYNC 10

SpiDevice::SpiDevice(spi_inst_t *spiBus, int baudRate, int dataBits, int clockPin, int txPin, int rxPin, int csPin, int clockMode) {
    _spiBus = spiBus;
    _baudRate = baudRate;
    _dataBits = dataBits;
    _clockPin = clockPin;
    _txPin = txPin;
    _rxPin = rxPin;
    _csPin = csPin;

    gpio_set_dir(_csPin, GPIO_OUT);
    gpio_put(_csPin, true);

    spi_init(_spiBus, _baudRate);
    spi_cpol_t cpolMode = SPI_CPOL_1;
    spi_cpha_t cphaseMode = SPI_CPHA_1;
    if (clockMode == 1) {
        cpolMode = SPI_CPOL_0;
    }
    if (clockMode == 2) {
        cphaseMode = SPI_CPHA_0;
    }
    spi_set_format( _spiBus, dataBits, cpolMode, cphaseMode, SPI_MSB_FIRST);

    gpio_set_function(_clockPin, GPIO_FUNC_SPI);
    gpio_set_function(_txPin, GPIO_FUNC_SPI);
    gpio_set_function(_rxPin, GPIO_FUNC_SPI);
    gpio_set_function(_csPin, GPIO_FUNC_SIO);

    sem_init(&_deviceLock, 1, 1);
}

SpiDevice::~SpiDevice() {
    spi_deinit(_spiBus);
    sem_reset(&_deviceLock, 1);
}

void SpiDevice::write(void *data, uint8_t length) {
    if (_txPin <= 0 || !spi_is_writable(_spiBus)) {
        // write disabled
        return;
    }

    sem_acquire_blocking(&_deviceLock);
    gpio_put(_csPin, false);
    if (_dataBits == 16)
        spi_write16_blocking(_spiBus, static_cast<uint16_t *>(data), length);
    else
        spi_write_blocking(_spiBus, static_cast<uint8_t *>(data), length);
    gpio_put(_csPin, true);
    sem_release(&_deviceLock);
}

uint32_t SpiDevice::read(void *buffer, uint8_t length) {
    if (_rxPin <= 0 || !spi_is_readable(_spiBus)) {
        // read disabled
        return 0;
    }

    uint32_t ret = 0;
    sem_acquire_blocking(&_deviceLock);
    gpio_put(_csPin, false);
    if (_dataBits == 16)
        ret = spi_read16_blocking(_spiBus, 0, static_cast<uint16_t *>(buffer), length);
    else
        ret = spi_read_blocking(_spiBus, 0, static_cast<uint8_t *>(buffer), length);
    gpio_put(_csPin, true);
    sem_release(&_deviceLock);
    return ret;

}
