
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Ssd1306.h"

#include <cstring>

#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_ENTIRE_ON 0xA4
#define SSD1306_SET_NORM_INV 0xA6
#define SSD1306_SET_DISP 0xAE
#define SSD1306_SET_MEM_ADDR 0x20
#define SSD1306_SET_COL_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22
#define SSD1306_SET_DISP_START_LINE 0x40
#define SSD1306_SET_SEG_REMAP 0xA0
#define SSD1306_SET_MUX_RATIO 0xA8
#define SSD1306_SET_COM_OUT_DIR 0xC0
#define SSD1306_SET_DISP_OFFSET 0xD3
#define SSD1306_SET_COM_PIN_CFG 0xDA
#define SSD1306_SET_DISP_CLK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_SET_VCOM_DESEL 0xDB
#define SSD1306_SET_CHARGE_PUMP 0x8D

#define SSD1306_COMMAND_START_BYTE 0x80
#define SSD1306_SHOW_CMD_START_BYTE 0x40

void Ssd1306::powerOn() {
    writeCommand(SSD1306_SET_DISP | 0x01);
}

void Ssd1306::powerOff() {
    writeCommand(SSD1306_SET_DISP | 0x00);
}

void Ssd1306::setContrast(uint8_t contrast) {
    writeCommand(SSD1306_SET_CONTRAST);
    writeCommand(contrast);
}

void Ssd1306::invertDisplay(bool invert) {
    if (invert == true) {
        writeCommand(SSD1306_SET_NORM_INV | 0x01);
    } else {
        writeCommand(SSD1306_SET_NORM_INV | 0x00);
    }
}

void Ssd1306::clear() {
    memset(_outBuffer + 1, 0, _pageBufferSize);
}

void Ssd1306::clearArea(int x, int y, int width, int height) {
    int maxX = x + width;
    int maxY = y + height;
    for (int yPos = y; yPos <= maxY; yPos++) {
        if (yPos >= _height)
            break;
        for (int xPos = x; xPos <= maxX; xPos++) {
            if (xPos >= _width)
                break;
            setPixelAt(xPos, yPos, false);
        }
    }
}

void Ssd1306::show() {
    if (_outBuffer == nullptr) {
        return;
    }

    int x0 = 0;
    int x1 = _width - 1;
    if (_width == 64) {
        // displays with width of 64 pixels are shifted by 32
        x0 += 32;
        x1 += 32;
    }
    writeCommand(SSD1306_SET_COL_ADDR);
    writeCommand(x0);
    writeCommand(x1);
    writeCommand(SSD1306_SET_PAGE_ADDR);
    writeCommand(0);
    writeCommand(_numPages - 1);

    // and send our display buffer
    _i2c->write(_address, _outBuffer, _pageBufferSize + 1);
}

void Ssd1306::writeFullScreenBitmap(const uint8_t *pageData, uint16_t pageDataSize) {
    if (_outBuffer == nullptr) {
        return;
    }

    for (int i = 0; i < pageDataSize && i < _pageBufferSize; i++) {
        _outBuffer[i + 1] = pageData[i];
    }

    show();
}

void Ssd1306::setPixelAt(int x, int y, bool color) {
    int page = (y >> 3) & 0x07;
    int pageBit = y % 8;
    // add 1 to skip the control byte at the start of the buffer
    int bufPos = (page * _width) + x + 1;
    if (color) {
        _outBuffer[bufPos] |= (1 << pageBit);
    } else {
        _outBuffer[bufPos] &= ~(1 << pageBit);
    }
}

void Ssd1306::deviceInit(HardwareI2C *i2c, int address) {
    _i2c = i2c;
    _address = address;
    _numPages = _height / 8;

    _pageBufferSize = _numPages * _width;
    _outBuffer = new uint8_t[_pageBufferSize + 1];
    _outBuffer[0] = SSD1306_SHOW_CMD_START_BYTE;

    writeCommand(SSD1306_SET_DISP);
    writeCommand(SSD1306_SET_MEM_ADDR);
    writeCommand(0x00); // horizontal address mode
    writeCommand(SSD1306_SET_DISP_START_LINE);
    writeCommand(SSD1306_SET_SEG_REMAP | 0x1);;
    writeCommand(SSD1306_SET_MUX_RATIO);
    writeCommand(_height - 1);
    writeCommand(SSD1306_SET_COM_OUT_DIR | 0x08);
    writeCommand(SSD1306_SET_DISP_OFFSET);
    writeCommand(0x00);
    writeCommand(SSD1306_SET_COM_PIN_CFG);
    if (_width > (2 * _height)) {
        writeCommand(0x02);
    } else {
        writeCommand(0x12);
    }
    writeCommand(SSD1306_SET_DISP_CLK_DIV);
    writeCommand(0x80);
    writeCommand(SSD1306_SET_PRECHARGE);
    writeCommand(0xF1);
    writeCommand(SSD1306_SET_VCOM_DESEL);
    writeCommand(0x30);
    writeCommand(SSD1306_SET_CONTRAST);
    writeCommand(0xFF);
    writeCommand(SSD1306_SET_ENTIRE_ON);
    writeCommand(SSD1306_SET_NORM_INV);
    writeCommand(SSD1306_SET_CHARGE_PUMP);
    writeCommand(0x14);
    writeCommand(SSD1306_SET_DISP | 0x01);
}

void Ssd1306::writeCommand(uint8_t command) const {
    uint8_t data[2] = {SSD1306_COMMAND_START_BYTE, command};
    _i2c->write(_address, data, 2);
}
