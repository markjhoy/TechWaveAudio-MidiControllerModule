
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Dac7554.h"

#include "hardware/gpio.h"
#include "../TechWaveAudio_MidiControllerModule.h"

#define DAC_7554_LD_CONTROL_BITS 0b1000000000000000

Dac7554::Dac7554(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin)
: SpiDevice(spiBus, baudRate, 16, clockPin, txPin, rxPin, csPin, 2) {
}

void Dac7554::writeValue(Dac7554Register outputRegister, uint16_t value) {
    uint16_t valueToUse = value;
    if (value < 0)
        valueToUse = 0;
    if (value >= DAC_7554_MAX_RANGE)
        valueToUse = DAC_7554_MAX_RANGE - 1;
    uint16_t writeValue = DAC_7554_LD_CONTROL_BITS | (outputRegister << 12) | valueToUse;
    write(&writeValue, 1);
}
