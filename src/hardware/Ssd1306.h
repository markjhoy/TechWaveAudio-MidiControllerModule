
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SSD1306_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SSD1306_H
#include "HardwareI2C.h"
#include "MonoLcdFramebuffer.h"

/**
 * A frame buffer implementation using a SSD1306 OLED screen
 */
class Ssd1306 : public MonoLcdFramebuffer {
public:
    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height)
        : MonoLcdFramebuffer(width, height) {
        auto scannedBus = i2c->scanBus();
        deviceInit(i2c, address);
    }

    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height, MonoLcdFramebufferMode mode)
        : MonoLcdFramebuffer(width, height, mode) {
        deviceInit(i2c, address);
    }

    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height, MonoLcdFramebufferMode mode, BaseCharacterSet *characterSet)
        : MonoLcdFramebuffer(width, height, mode) {
        deviceInit(i2c, address);
    }

    ~Ssd1306() override {
        delete _outBuffer;
    }

    /**
     * Powers the device on
     */
    void powerOn();

    /**
     * Powers the device off
     */
    void powerOff();

    void initialize() { deviceInit(_i2c, _address); }

    /**
     * Sets the contract for the screen
     * @param contrast the contrast level
     */
    void setContrast(uint8_t contrast);

    /**
     * Inverts the display screen
     * @param invert to invert or not
     */
    void invertDisplay(bool invert);

    /**
     * Clears the display
     */
    void clear() override;

    /**
     * Clears a selected portion of the display
     * @param x the x pixel to start at
     * @param y the y pixel to start at
     * @param width the width of the area to clear
     * @param height the height of the area to clear
     */
    void clearArea(int x, int y, int width, int height) override;

    /**
     * Send the frame buffer data to the display to show it
     */
    void show() override;

    void writeFullScreenBitmap(const uint8_t *pageData, uint16_t pageDataSize);

protected:

    /**
     * Sets a pixel in the internal buffer for the display.
     * We work in pages, so the standard frame buffer is not used, but rather we
     * translate the coordinates and set / clear the appropriate bits in our
     * own buffer.
     * @param x the x position
     * @param y the y position
     * @param color the color value (on / off)
     */
    void setPixelAt(int x, int y, bool color) override;

private:
    HardwareI2C *_i2c = nullptr;
    uint8_t _address = 0x00;
    uint8_t *_outBuffer = nullptr;

    int _numPages = 0;
    int _pageBufferSize = 0;

    void deviceInit(HardwareI2C *i2c, int address);
    void writeCommand(uint8_t command) const;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SSD1306_H