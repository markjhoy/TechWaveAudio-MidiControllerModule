
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
#include <string>

#include "../common/ImageSet.h"
#include "../hardware/Ssd1306.h"

/**
 * Our interface to the OLED Display
 * The interface keeps a display text buffer behind the scenes (no graphics)
 * and is written to the device when the `show` function is called.
 */
class OledDisplay {
public:
    explicit OledDisplay(HardwareI2C *i2c);
    ~OledDisplay();

    /**
     * Gets the text character sizing in pixels
     * @return The text character sizing
     */
    [[nodiscard]] BoxSize getTextCharacterSizing(OledFontType font) const {
        return _lcd->getTextCharacterSizing(font);
    }

    /**
     * Clears the display buffer
     * @param refresh immediate send data to clear the display
     */
    void clear(bool refresh = true);

    /**
     * Clears an area of the screen based on text sizings
     * @param x the text X position
     * @param y the text Y position
     * @param numChars the number of characters for the width
     * @param font
     */
    void clearTextArea(int x, int y, int numChars, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Clears out a single line with space characters
     * @param lineNumber the line number to clear
     */
    void clearLine(int lineNumber, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Clears a rectangular area on the screen
     * @param r the rectangular area to clear
     */
    void clearArea(const ScreenRectangle &r) {
        clearArea(r.xPos, r.yPos, r.width, r.height);
    }

    /**
     * Clears a rectangular area on the screen
     * @param x starting x position
     * @param y starting y position
     * @param width the width
     * @param height the height
     */
    void clearArea(int x, int y, int width, int height);

    /**
     * Finalizes and displays the contents of the text buffer to the I2C device
     * @param clearDisplay clears the display before writing out the text buffer
     */
    void show(bool clearDisplay = true);

    /**
     * Sets the title of the display
     * @param title the title text to set
     */
    void setTitle(const std::string &title);

    /**
     * Displays the boot screen
     */
    void displayBootScreen();

    /**
     * Copies image data to the screen
     * @param x the x position to start
     * @param y the y position to start
     * @param width the width of the image
     * @param data pointer to the image data
     * @param numDataBytes the number of bytes in the image
     */
    void blitImage(int x, int y, uint8_t width, const uint8_t *data, uint16_t numDataBytes) const;

    /**
     * Writes text data starting at position (0,0) and wrapping around
     * until all characters are written or the text buffer is full.
     * @param lines pointer to the lines to write
     * @param length the total length (in characters) of the lines
     */
    void writeLines(char const *lines, int length, bool highlightFirstLine = true, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Writes lines starting at line 0. One line per passed in string.
     * Always highlights the first line (line 0)
     * @param lines pointer to an array of std::string
     * @param numLines the number of strings in the array
     */
    void writeLines(const std::string *lines, int numLines, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Writes a highlighted title, and up to three additional lines.
     * @param title the title string
     * @param lines pointer to an array of std::string
     * @param numLines number of strings in the array
     * @param font
     */
    void writeLines(const std::string &title, const std::string *lines, int numLines, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Writes a single string at the given line, optionally highlighting it.
     * @param lineNumber the line number to write at
     * @param line the actual string to write
     * @param highlight true to highlight the line or not (default = false)
     */
    void writeLineAt(int lineNumber, const std::string &line, bool highlight = false, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Writes text at an arbitrary location in the buffer. Does not wrap lines.
     * Will clear the background of the area where it writes the text
     * @param x the x position in the text buffer
     * @param y the y position in the text buffer
     * @param text the text to write
     * @param writeDirect pass true to immediately write the text to the underlying device as well
     */
    void writeTextAt(int x, int y, const std::string &text, OledFontType font = OLED_DEFAULT_FONT, bool highlight = false);

    /**
     * Writes text starting at a particular line number. Will wrap the text to the next line
     * @param lineNumber the starting line number
     * @param text the text to display
     * @param font the font to use
     */
    void writeTextStartingAtLine(int lineNumber, const std::string &text, OledFontType font = OLED_DEFAULT_FONT);

    /**
     * Writes text at an arbitrary place directly on the screen
     * @param x the starting x position
     * @param y the starting y position
     * @param str the text to write
     * @param font the font to use
     */
    void writeTextString(int x, int y, const std::string &str, const OledFontType font = OLED_DEFAULT_FONT) const {
        _lcd->writeTextString(x, y, str, true, font);
    }

    /**
     * Draws an optionally filled in rectangle to the screen
     * @param x the screen X position
     * @param y the screen Y position
     * @param width the pixel width
     * @param height the pixel height
     * @param color the color value for the rectangle and optional fill
     * @param fill true to fill in the rectangle, false to not.
     */
    void drawRect(int x, int y, int width, int height, bool color, bool fill);

    /**
     * Displays a list menu with current selection highlighted, and optionally a
     * chosen item in the menu (marked with an asterisk `*`).
     * @param title the menu title
     * @param menuItems pointer to an array of std::string containing the menu items
     * @param currentItem the current item selected in the array
     * @param numMenuItems the total number of items in the array
     * @param selectedItem any chosen menu item to mark with a `*` character, or -1 for none (default = -1)
     */
    void showMenu(const std::string &title, std::string *menuItems, int currentItem, int numMenuItems, int selectedItem = -1);

    /**
     * Powers off the display
     */
    void powerOff() const { _lcd->powerOff();  }

    /**
     * Powers the display on
     */
    void powerOn() const {
        _lcd->powerOn();
        _lcd->initialize();
    }

private:
    Ssd1306 *_lcd = nullptr;
    BoxSize _screenSize{};
    const int _maxCharactersDisplay = OLED_NUM_CHARS_PER_LINE * OLED_MAX_NUM_TEXT_LINES;

    ImageSet *_bootScreenImageSet = nullptr;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
