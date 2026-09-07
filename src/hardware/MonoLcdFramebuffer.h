
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H
#include "../display/BaseCharacterSet.h"
#include "../TechWaveAudio_MidiControllerModule.h"

/**
 * Base class for a framebuffer for a monochrome LCD / OLED device.
 * All pixels plotted in the frame buffer use the overridable `setPixelAt` function.
 */
class MonoLcdFramebuffer {
public:
    explicit MonoLcdFramebuffer(int width, int height);
    explicit MonoLcdFramebuffer(int width, int height, MonoLcdFramebufferMode mode);

    virtual ~MonoLcdFramebuffer();

    /**
     * Writes a text string to a specific area in the frame buffer. No text wrapping occurs,
     * @param x the x pixel to start at
     * @param y the y pixel to start at
     * @param message The text string to write
     * @param color the color value (on / off)
     */
    void writeTextString(int x, int y, const std::string &message, bool color);
    void writeTextString(int x, int y, const std::string &message, bool color, OledFontType font);

    /**
     * Writes a text pointer buffer to a specific area in the frame buffer.
     * Text wrapping with occur if the end of the screen width is reached.
     * @param x the x pixel to start at
     * @param y the y pixel to start at
     * @param buffer pointer to the character buffer
     * @param length length of the text to write
     * @param color the color value (on / off)
     */
    void writeTextBuffer(int x, int y, const char *buffer, int length, bool color);
    void writeTextBuffer(int x, int y, const char *buffer, int length, bool color, OledFontType font);

    /**
     * Draws a rectangle outline on the frame buffer
     * @param x the x starting point
     * @param y the y starting point
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @param color the color value (on / off)
     */
    void rect(int x, int y, int width, int height, bool color);

    /**
     * Draws an optionally filled rectangle on the frame buffer
     * @param x the x starting point
     * @param y the y starting point
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @param color the color value (on / off)
     * @param fill true to fill the rectangle with the color
     */
    void rect(int x, int y, int width, int height, bool color, bool fill);

    /**
     * Draws a line to the frame buffer
     * @param x1 the x starting point
     * @param y1 the y starting point
     * @param x2 the x ending point
     * @param y2 the y ending point
     * @param color the color value (on / off)
     */
    void line(int x1, int y1, int x2, int y2, bool color);

    /**
     * Plots a single pixel to the frame buffer
     * @param x the x position
     * @param y the y position
     * @param color the color value (on / off)
     */
    void setPixel(int x, int y, bool color);

    /**
     * Clears the frame buffer. Optionally overridable so the device can clear its own contents.
     */
    virtual void clear();

    /**
     * Clears a rectangular area for the framebuffer. Optionally overridable so the
     * implementing class can handle its own area clearing.
     * @param x the x pixel to start at
     * @param y the y pixel to start at
     * @param width the width of the area to clear
     * @param height the height of the area to clear
     */
    virtual void clearArea(int x, int y, int width, int height);

    /**
     * Base show function. The specific device class should call this to display the current framebuffer.
     */
    virtual void show() = 0;

    /**
     * Gets the width and height of the frame buffer
     * @return The screen sizing
     */
    [[nodiscard]] BoxSize getScreenSize() const { return { _width, _height }; }

    /**
     * Gets the width and height of the character set in use
     * @return the dimensions of a character
     */
    [[nodiscard]] BoxSize getTextCharacterSizing() const {
        return getTextCharacterSizing(OLED_DEFAULT_FONT);
    }

    [[nodiscard]] BoxSize getTextCharacterSizing(OledFontType font) const {
        return  { _charsets[font]->getCharacterWidth(), _charsets[font]->getCharacterHeight() };
    }

protected:
    // the frame buffer width
    int _width = 0;
    // the frame buffer height
    int _height = 0;
    // the mode for rendering the rame buffer
    MonoLcdFramebufferMode _mode = LCD_FRAMEBUFFER_MODE_HORIZONTAL;

    // our internal framebuffer
    uint8_t *_framebuffer = nullptr;

    std::vector<BaseCharacterSet*> _charsets{};

    /**
     * Overridable function to set a pixel at a set of coordinates.
     * Hardware devices may not want to use the internal framebuffer
     * @param x the x position
     * @param y the y position
     * @param color the color value (on / off)
     */
    virtual void setPixelAt(int x, int y, bool color) { _framebuffer[(y * _width) + x] = color; }

private:
    void clearCharSets();
    void internal_init(int width, int height, MonoLcdFramebufferMode mode);

    void setPixelsFromByte(int xStart, int y, uint8_t byte, bool color);
    void plotCharAt(unsigned char code, int x, int y, bool color, OledFontType font);
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MONOLCDFRAMEBUFFER_H