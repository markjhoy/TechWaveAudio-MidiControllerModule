/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-03.
//

#include "NoteVelOut1Out2Output.h"

NoteVelOut1Out2Output::NoteVelOut1Out2Output(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin,int csPin)
: Dac7554(spiBus, baudRate, clockPin, txPin, rxPin, csPin) {
    writeNote(0);
    writeVelocity(0);
    writeOut1(0);
    writeOut2(0);
}

void NoteVelOut1Out2Output::writeNote(uint16_t value) {
    this->writeValue(DAC7554_REGISTER_A, value);
}

void NoteVelOut1Out2Output::writeVelocity(uint16_t value) {
    this->writeValue(DAC7554_REGISTER_B, value);
}

void NoteVelOut1Out2Output::writeOut1(uint16_t value) {
    this->writeValue(DAC7554_REGISTER_C, value);
}

void NoteVelOut1Out2Output::writeOut2(uint16_t value) {
    this->writeValue(DAC7554_REGISTER_D, value);
}
