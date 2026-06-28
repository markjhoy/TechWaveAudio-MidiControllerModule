
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H
#include "BaseCharacterSet.h"
#include "../TechWaveAudio_MidiControllerModule.h"


class MonoLcdFramebuffer {
public:
    explicit MonoLcdFramebuffer(int width, int height);
    explicit MonoLcdFramebuffer(int width, int height, MonoLcdFramebufferMode mode);
    explicit MonoLcdFramebuffer(int width, int height, MonoLcdFramebufferMode mode, BaseCharacterSet *characterSet);

    virtual ~MonoLcdFramebuffer();

    void text(const std::string &message, int x, int y, bool color);
    void writeTextBuffer(int x, int y, const char *buffer, int length, bool color);
    void rect(int x, int y, int width, int height, bool color);
    void rect(int x, int y, int width, int height, bool color, bool fill);
    void line(int x1, int y1, int x2, int y2, bool color);
    void setPixel(int x, int y, bool color);

    virtual void clear();
    virtual void clearArea(int x, int y, int width, int height);
    virtual void show() {}

    [[nodiscard]] BoxSize getScreenSize() const { return { _width, _height }; }
    [[nodiscard]] BoxSize getTextCharacterSizing() const { return  { _charsetWidth, _charsetHeight }; }

protected:
    int _width = 0;
    int _height = 0;
    MonoLcdFramebufferMode _mode = LCD_FRAMEBUFFER_MODE_HORIZONTAL;
    int _charsetWidth = 0;
    int _charsetHeight = 0;
    uint8_t *_charsetBytes = nullptr;
    int _charsetWidthBytes = 0;
    int _charsetBytesPerChar = 0;

    uint8_t *_framebuffer = nullptr;

    void internal_init(int width, int height, MonoLcdFramebufferMode mode, BaseCharacterSet *characterSet, bool deleteCharSet);

    void setPixelsFromByte(int xStart, int y, uint8_t byte, bool color);
    void plotCharAt(unsigned char code, int x, int y, bool color);

    virtual void setPixelAt(int x, int y, bool color) { _framebuffer[(y * _width) + x] = color; }
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H