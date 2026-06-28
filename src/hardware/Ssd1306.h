
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

class Ssd1306 : public MonoLcdFramebuffer {
public:
    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height)
        : MonoLcdFramebuffer(width, height) {
        init(i2c, address);
    }

    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height, MonoLcdFramebufferMode mode)
        : MonoLcdFramebuffer(width, height, mode) {
        init(i2c, address);
    }

    Ssd1306(HardwareI2C *i2c, uint8_t address, int width, int height, MonoLcdFramebufferMode mode, BaseCharacterSet *characterSet)
        : MonoLcdFramebuffer(width, height, mode, characterSet) {
        init(i2c, address);
    }

    ~Ssd1306() override {
        delete _outBuffer;
        delete _tempBuffer;
    }

    void powerOn();
    void powerOff();
    void setContrast(uint8_t contrast);
    void invertDisplay(bool invert);

    void clear() override;
    void clearArea(int x, int y, int width, int height) override;
    void show() override;

protected:
    void setPixelAt(int x, int y, bool color) override;
private:
    HardwareI2C *_i2c = nullptr;
    uint8_t _address = 0x00;
    uint8_t *_tempBuffer = new uint8_t[2];
    uint8_t *_outBuffer = nullptr;

    int _numPages = 0;
    int _pageBufferSize = 0;

    void init(HardwareI2C *i2c, int address);

    void writeCommand(uint8_t command);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SSD1306_H