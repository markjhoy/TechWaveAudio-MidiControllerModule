//
// Created by Mark J. Hoy on 2026-05-03.
//

#include "Mcp4725.h"

Mcp4725::Mcp4725(HardwareI2C *i2c, int8_t address) {
    _i2c = i2c;
    _address = address;
    _buffer = new uint8_t[2];
}

Mcp4725::~Mcp4725() {
    delete[] _buffer;
}

void Mcp4725::write(int value) {
    int valueToWrite = value;
    if (value < 0) {
        valueToWrite = 0;
    } else if (value > 4095) {
        valueToWrite = 4095;
    }

    _buffer[0] = 0x00 | ((valueToWrite >> 8) & 0x0F);
    _buffer[1] = valueToWrite & 0xFF;
    _i2c->write(_address, _buffer, 2);
}
