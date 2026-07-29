
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Mcp4902.h"
#include "../TechWaveAudio_MidiControllerModule.h"

#include "hardware/gpio.h"

void Mcp4902::writeValue(Mcp4902Register outputRegister, uint value) {
    uint8_t valueToUse = value;
    if (value < 0)
        valueToUse = 0;
    if (value > DAC_4902_MAX_RANGE)
        valueToUse = DAC_4902_MAX_RANGE;
    _buffer[0] = ((outputRegister == Mcp4902_REGISTER_A) ? 0x30 : 0xB0) + ((valueToUse >> 4) & 0x0F);
    _buffer[1] = ((valueToUse << 4) & 0xF0);
    this->write(_buffer, 2);
}
