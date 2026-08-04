
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Dac7554.h"

#include "hardware/gpio.h"
#include "../TechWaveAudio_MidiControllerModule.h"

void Dac7554::writeValue(Dac7554Register outputRegister, uint16_t value) {
    uint16_t valueToUse = value;
    if (value < 0)
        valueToUse = 0;
    if (value >= DAC_7554_MAX_RANGE)
        valueToUse = DAC_7554_MAX_RANGE - 1;
    _buffer[0] = 0b10000000 | ((outputRegister << 6) & 0x0F) | ((valueToUse >> 8) & 0xFF);
    _buffer[1] = valueToUse & 0xFF;
    write(_buffer, 2);
}
