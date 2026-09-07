
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MonoLcdFramebuffer.h"

#include <cstring>

#include "../display/CharacterMapping8x16.h"
#include "../display/CharacterMapping8x8.h"

MonoLcdFramebuffer::MonoLcdFramebuffer(int width, int height) {
    internal_init(width, height, LCD_FRAMEBUFFER_MODE_HORIZONTAL);
}

MonoLcdFramebuffer::MonoLcdFramebuffer(int width, int height, MonoLcdFramebufferMode mode) {
    internal_init(width, height, mode);
}

MonoLcdFramebuffer::~MonoLcdFramebuffer() {
    clearCharSets();
    delete _framebuffer;
}

void MonoLcdFramebuffer::writeTextString(int x, int y, const std::string &message, bool color) {
    writeTextString(x, y, message, color, OLED_DEFAULT_FONT);
}

void MonoLcdFramebuffer::writeTextString(int x, int y, const std::string &message, bool color, OledFontType font) {
    if (message.empty()) {
        return;
    }

    int currentX = x;
    int charWidth = _charsets[font]->getCharacterWidth();
    for (int c = 0; c < message.length() && currentX < _width; c++) {
        plotCharAt(message[c], currentX, y, color, font);
        currentX += charWidth;
    }
}

void MonoLcdFramebuffer::writeTextBuffer(int x, int y, const char *buffer, int length, bool color) {
    writeTextBuffer(x, y, buffer, length, color, OLED_DEFAULT_FONT);
}

void MonoLcdFramebuffer::writeTextBuffer(int x, int y, const char *buffer, int length, bool color, OledFontType font) {
    if (buffer == nullptr || length == 0) {
        return;
    }

    int bufferPos = 0;
    int yPos = y;
    int charHeight = _charsets[font]->getCharacterHeight();
    int charWidth = _charsets[font]->getCharacterWidth();
    while (yPos < _height && bufferPos < length) {
        for (int xPos = x; xPos < _width && bufferPos < length; xPos += charWidth) {
            plotCharAt(buffer[bufferPos++], xPos, yPos, color, font);
        }
        yPos += charHeight;
    }
}

void MonoLcdFramebuffer::rect(int x, int y, int width, int height, bool color) {
    rect(x, y, width, height, color, false);
}

void MonoLcdFramebuffer::rect(int x, int y, int width, int height, bool color, bool fill) {
    // TODO -- optimize this

    int x2 = x + width;
    int y2 = y + height;
    for (int yPos = y; yPos < y2; yPos++) {
        for (int xPos = x; xPos < x2; xPos++) {
            if (fill || xPos == x || yPos == y || xPos == (x2-1) || yPos == (y2-1)) {
                setPixel(xPos, yPos, color);
            }
        }
    }
}

void MonoLcdFramebuffer::line(int x1, int y1, int x2, int y2, bool color) {
    int fromX = std::min(x1, x2);
    int toX = std::max(x1, x2);

    if (fromX == toX) {
        int fromY = std::min(y1, y2);
        int toY = std::max(y1, y2);
        for (int y = fromY; y <= toY; y++) {
            setPixel(fromX, y, color);
        }
        return;
    }

    int dx = x2 - x1;
    int dy = y2 - y1;
    float slope = (float) dy / (float) dx;
    float yVal = float(y1);
    for (int xPos = fromX; xPos <= toX; xPos++) {
        setPixel(xPos, int(yVal), color);
        yVal += slope;
    }
}

void MonoLcdFramebuffer::clear() {
    memset(_framebuffer, 0, _width * _height);
}

void MonoLcdFramebuffer::clearArea(int x, int y, int width, int height) {
    int fromX = std::max(x, 0);
    int fromY = std::max(y, 0);
    int toX = std::min(x + width, _width);
    int toY = std::min(y + height, _height);

    if (fromX == 0 && toX == _width) {
        int blockSize = (toY * _width) - (fromY * _width);
        uint8_t *blockStart = (_framebuffer + (fromY * _width));
        memset(blockStart, 0, blockSize);
        return;
    }

    for (int yPos = fromY; yPos <= toY; yPos++) {
        uint32_t offset = x + (yPos * _width);
        memset(_framebuffer + offset, 0, (toX - fromX + 1));
    }
}

void MonoLcdFramebuffer::clearCharSets() {
    for (auto font : _charsets) {
        delete font;
    }
    _charsets.clear();
}

void MonoLcdFramebuffer::internal_init(int width, int height, MonoLcdFramebufferMode mode) {
    clearCharSets();

    BaseCharacterSet *oledFonts[OLED_NUM_FONTS] = OLED_FONT_MAPPINGS;
    for (auto font : oledFonts) {
        _charsets.push_back(font);
    }

    _width = width;
    _height = height;
    _mode = mode;

    int frmeBufferSize = width * height;
    _framebuffer = new uint8_t[frmeBufferSize];
    memset(_framebuffer, 0, frmeBufferSize);
}

void MonoLcdFramebuffer::setPixel(int x, int y, bool color) {
    if (x < 0 || x >= _width || y < 0 || y >= _height) {
        return;
    }
    setPixelAt(x, y, color);
}

void MonoLcdFramebuffer::setPixelsFromByte(int xStart, int y, uint8_t byteVal, bool color) {
    if (xStart < 0 || xStart >= _width || y < 0 || y >= _height || byteVal == 0x00) {
        return;
    }
    int xPos = xStart;
    for (int mb = 0; mb < 8; mb++) {
        if (xPos >= _width) {
            return;
        }
        if ((byteVal & (1 << mb)) > 0) {
            setPixelAt(xPos, y, color);
        }
        ++xPos;
    }
}

void MonoLcdFramebuffer::plotCharAt(unsigned char code, int x, int y, bool color, OledFontType font) {
    if (code < 0x20 || code > 0x7f) {
        return;
    }

    int maxYIndex = y + _charsets[font]->getCharacterHeight();
    int index = (code - 0x20) * _charsets[font]->getBytesPerChar();
    uint8_t *charBufferPos = _charsets[font]->getCharacterArray() + index;
    int charsetWidthBytes = _charsets[font]->getCharacterWidthInBytes();
    for (int yIndex = y; yIndex < maxYIndex; yIndex++) {
        int xPos = x;
        for (int cw = 0; cw < charsetWidthBytes; cw++) {
            setPixelsFromByte(xPos, yIndex, (*charBufferPos), color);
            xPos += 8;
            charBufferPos++;
        }
    }
}
