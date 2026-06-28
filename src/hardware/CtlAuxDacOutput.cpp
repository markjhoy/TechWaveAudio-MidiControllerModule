
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

void CtlAuxDacOutput::writeAux(const uint8_t value) {
    sendBuffer(DAC_4902_AUX_HIGH_BYTE, value);
}

void CtlAuxDacOutput::writeCtl(const uint8_t value) {
    sendBuffer(DAC_4902_CTL_HIGH_BYTE, value);
}

void CtlAuxDacOutput::sendBuffer(const uint8_t highByteStart, const uint8_t value) {
    uint8_t highByte = highByteStart;
    highByte |= ((value >> 4) % 0x0F);

    _buffer[0] = highByte;
    _buffer[1] = (value << 4) & 0xF0;

    write(_buffer, 2);
}
