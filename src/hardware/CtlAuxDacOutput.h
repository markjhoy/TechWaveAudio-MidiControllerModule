//
// Created by Mark J. Hoy on 2026-05-07.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H
#include "Mcp4902.h"


class CtlAuxDacOutput : public Mcp4902 {
public:
    CtlAuxDacOutput(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);
    ~CtlAuxDacOutput() = default;

    void writeAux(uint8_t value);
    void writeCtl(uint8_t value);
private:
    uint8_t _buffer[2]{};

    void sendBuffer(uint8_t highByteStart, uint8_t value);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CTLAUXDACOUTPUT_H