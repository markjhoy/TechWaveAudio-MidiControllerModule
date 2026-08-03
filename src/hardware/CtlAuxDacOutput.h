
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#include "Mcp4902.h"

/**
 * Our interface for handling the aux and control outputs via a MCP4902 (dual channel DAC, SPI)
 */
class CtlAuxDacOutput : public Mcp4902 {
public:
    CtlAuxDacOutput(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);

    /**
     * Writes a value to the AUX line
     * @param value the value to write
     */
    void writeAux(int value);

    /**
     * Writes a value to the CONTROL line
     * @param value the value to write
     */
    void writeCtl(int value);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H