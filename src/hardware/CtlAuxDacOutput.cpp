
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "CtlAuxDacOutput.h"
#include "../TechWaveAudio_MidiControllerModule.h"

CtlAuxDacOutput::CtlAuxDacOutput(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin)
: Mcp4902(spiBus, baudRate, clockPin, txPin, rxPin, csPin) {
    writeAux(0);
    writeCtl(0);
}

void CtlAuxDacOutput::writeAux(const int value) {
    sendBuffer(DAC_4902_AUX_HIGH_BYTE, value);
}

void CtlAuxDacOutput::writeCtl(const int value) {
    sendBuffer(DAC_4902_CTL_HIGH_BYTE, value);
}

void CtlAuxDacOutput::sendBuffer(const uint8_t highByteStart, const int value) {
    auto valueToUse = static_cast<uint8_t>(value);
    if (value < 0) {
        valueToUse = 0;
    } else if (value > 255) {
        valueToUse = 255;
    }

    uint8_t highByte = highByteStart;
    highByte |= ((valueToUse >> 4) & 0x0F);

    _buffer[0] = highByte;
    _buffer[1] = (valueToUse << 4) & 0xF0;

    write(_buffer, 2);
}
