
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

void CtlAuxDacOutput::writeAux(const uint value) {
    this->writeValue(Mcp4902_REGISTER_A, value);
}

void CtlAuxDacOutput::writeCtl(const uint value) {
    this->writeValue(Mcp4902_REGISTER_B, value);
}

