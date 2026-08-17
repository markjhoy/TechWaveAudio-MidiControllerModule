/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_NOTEVELOUT1OUT2OUTPUT_H
#define TECHWAVEAUDIO_MCM_V2_0_0_NOTEVELOUT1OUT2OUTPUT_H

#include "Dac7554.h"

/**
 * Outputs for the base note, velocity, Out1 and OUt2 CV outputs
 */
class NoteVelOut1Out2Output : public Dac7554 {
public:
    NoteVelOut1Out2Output(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);

    void writeNote(uint16_t value);
    void writeVelocity(uint16_t value);
    void writeOut1(uint16_t value);
    void writeOut2(uint16_t value);
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_NOTEVELOUT1OUT2OUTPUT_H