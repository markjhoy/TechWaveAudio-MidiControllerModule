
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#include "Mcp4902.h"


class CtlAuxDacOutput : public Mcp4902 {
public:
    CtlAuxDacOutput(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);
    ~CtlAuxDacOutput() = default;

    void writeAux(int value);
    void writeCtl(int value);
private:
    uint8_t _buffer[2]{};

    void sendBuffer(uint8_t highByteStart, int value);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H