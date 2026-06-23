//
// Created by Mark J. Hoy on 2026-05-07.
//

#include "Mcp4902.h"

#include "hardware/gpio.h"
#include "hardware/spi.h"

Mcp4902::Mcp4902(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin) {
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

Mcp4902::~Mcp4902() {
    spi_deinit(_spiBus);
}

void Mcp4902::write(uint8_t *data, uint8_t size) {
    if (_txPin <= 0 || !spi_is_writable(_spiBus)) {
        // write disabled
        return;
    }

    gpio_put(_csPin, false);
    spi_write_blocking(_spiBus, data, size);
    gpio_put(_csPin, true);
}

uint8_t Mcp4902::read(uint8_t *buffer, uint8_t maxSize) {
    if (_rxPin <= 0 || !spi_is_readable(_spiBus)) {
        // read disabled
        return 0;
    }

    gpio_put(_csPin, false);
    uint8_t numRead = spi_read_blocking(_spiBus, 0, buffer, maxSize);
    gpio_put(_csPin, true);
    return numRead;
}

