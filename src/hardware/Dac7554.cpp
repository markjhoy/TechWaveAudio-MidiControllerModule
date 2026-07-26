
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Dac7554.h"

#include "hardware/gpio.h"

void Dac7554::writeValue(Dac7554Register outputRegister, uint16_t value) {
    _buffer[0] = 0b10000000 | ((outputRegister << 6) & 0x0F) | ((value >> 8) & 0xFF);
    _buffer[1] = value & 0xFF;
    write(_buffer, 2);
}
