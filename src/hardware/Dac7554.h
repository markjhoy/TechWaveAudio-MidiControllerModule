
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_DAC7554_H
#define TECHWAVEAUDIO_MCM_DAC7554_H
#include "SpiDevice.h"
#include "hardware/spi.h"
#include "../TechWaveAudio_MidiControllerModule.h"

/**
 * SPI driven interface to a DAC7554 12-bit quad DAC
 */
class Dac7554 : public SpiDevice {
public:
    /**
     * Creates a DAC7554 device object
     * @param spiBus the SPI bus for data transmission
     * @param baudRate (max up to 50 MHz theoretical limit)
     * @param clockPin the SPI clock pin
     * @param txPin the SPI write pin
     * @param rxPin the SPI read pin (not used)
     * @param csPin the chip select enable pin
     */
    Dac7554(spi_inst_t *spiBus, int baudRate, int clockPin, int txPin, int rxPin, int csPin);

    /**
     * Writes a value out on the SPI interface to the device
     * @param outputRegister the register to write to
     * @param value the value (max 12 bits)
     */
    void writeValue(Dac7554Register outputRegister, uint16_t value);
private:
    uint16_t _buffer[2]{};
};


#endif //TECHWAVEAUDIO_MCM_DAC7554_H