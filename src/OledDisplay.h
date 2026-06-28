
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
#include <string>

#include "hardware/Ssd1306.h"

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
    [[nodiscard]] BoxSize getTextCharacterSizing() const { return _charSize; }

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
     */
    void clearTextArea(int x, int y, int numChars);

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
     * Writes text data starting at position (0,0) and wrapping around
     * until all characters are written or the text buffer is full.
     * @param lines pointer to the lines to write
     * @param length the total length (in characters) of the lines
     */
    void writeLines(char const *lines, int length, bool highlightFirstLine = true);

    /**
     * Writes lines starting at line 0. One line per passed in string.
     * Always highlights the first line (line 0)
     * @param lines pointer to an array of std::string
     * @param numLines the number of strings in the array
     */
    void writeLines(const std::string *lines, int numLines);

    /**
     * Writes a highlighted title, and up to three additional lines.
     * @param title the title string
     * @param lines pointer to an array of std::string
     * @param numLines number of strings in the array
     */
    void writeLines(const std::string &title, const std::string *lines, int numLines);

    /**
     * Writes a single string at the given line, optionally highlighting it.
     * @param lineNumber the line number to write at
     * @param line the actual string to write
     * @param highlight true to highlight the line or not (default = false)
     */
    void writeLineAt(int lineNumber, const std::string &line, bool highlight = false);

    /**
     * Writes text at an arbitrary location in the buffer. Does not wrap lines.
     * @param x the x position in the text buffer
     * @param y the y position in the text buffer
     * @param text the text to write
     * @param writeDirect pass true to immediately write the text to the underlying device as well
     */
    void writeTextAt(int x, int y, const std::string &text);

    /**
     * Clears out a single line with space characters
     * @param lineNumber the line number to clear
     */
    void clearLine(int lineNumber);

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

private:
    Ssd1306 *_lcd = nullptr;
    BoxSize _charSize{};
    BoxSize _screenSize{};
    const int _maxCharactersDisplay = OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_LCDDISPLAY_H
