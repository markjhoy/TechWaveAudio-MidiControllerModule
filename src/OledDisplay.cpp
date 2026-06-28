
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OledDisplay.h"

#include <cstring>

OledDisplay::OledDisplay(HardwareI2C *i2c) {
    _lcd = new Ssd1306(i2c, OLED_ADDRESS, OLED_DISPLAY_WIDTH, OLED_DISPLAY_HEIGHT);
    _screenSize = _lcd->getScreenSize();
    _charSize = _lcd->getTextCharacterSizing();
}

OledDisplay::~OledDisplay() {
    delete _lcd;
}

void OledDisplay::clear(bool refresh) {
    _lcd->clear();
    if (refresh) {
        _lcd->show();
    }
}

void OledDisplay::clearTextArea(int x, int y, int numChars) {
    int xPos = x * _charSize.width;
    int yPos = y * _charSize.height;
    int textWidth = numChars * _charSize.width;
    _lcd->clearArea(xPos, yPos, textWidth, _charSize.height);
}

void OledDisplay::show(bool clearDisplay) {
    _lcd->show();
}

void OledDisplay::setTitle(const std::string &title) {
    writeLineAt(0, title, false);
}

void OledDisplay::displayBootScreen() {
    clear(true);
    writeLines(boot_screen_lines, 4);
    show();
}

void OledDisplay::writeLines(char const *lines, int length, bool highlightFirstLine) {
    clear(false);
    int maxNumChars = std::min(length, _maxCharactersDisplay);
    int yPos = 0;
    uint32_t currentOffset = 0;
    while (currentOffset < maxNumChars) {
        int numCharsThisLine = std::min(static_cast<int>(length - currentOffset), 16);
        if (currentOffset == 0 && highlightFirstLine) {
            // highlight the first line
            _lcd->rect(0, 0, _screenSize.width, _screenSize.height, true, true);
            _lcd->writeTextBuffer(0, 0, lines + currentOffset, numCharsThisLine, false);
        } else {
            _lcd->writeTextBuffer(0, yPos, lines + currentOffset, numCharsThisLine, true);
        }
        yPos += _charSize.height;
        currentOffset += numCharsThisLine;
    }
    show();
}

void OledDisplay::writeLines(const std::string *lines, int numLines) {
    for (int i = 0; i < numLines && i < 4; i++) {
        writeLineAt(i, lines[i], i==0);
    }
}

void OledDisplay::writeLines(const std::string &title, const std::string *lines, int numLines) {
    setTitle(title);
    for (int i = 1; i < numLines; i++) {
        writeLineAt(i, lines[i]);
    }
}

void OledDisplay::writeLineAt(int lineNumber, const std::string &line, bool highlight) {
    if (lineNumber < 0 || lineNumber >= OLED_NUM_TEXT_LINES) {
        return;
    }

    auto charSize = _lcd->getTextCharacterSizing();
    int yPos = lineNumber * charSize.height;
    int lineLen = std::min(static_cast<int>(line.length()), OLED_NUM_CHARS_PER_LINE);
    auto linePtr = line.data();
    _lcd->clearArea(0, yPos, _screenSize.width, _charSize.height);
    if (highlight) {
        _lcd->rect(0, yPos, _screenSize.width, _charSize.height, true, true);
    }
    _lcd->writeTextBuffer(0, yPos, linePtr, lineLen, !highlight);
}

void OledDisplay::writeTextAt(int x, int y, const std::string &text) {
    if (x < 0 || x > OLED_NUM_CHARS_PER_LINE || y < 0 || y > OLED_NUM_TEXT_LINES) {
        return;
    }

    int maxLength = std::min((int)text.length(), OLED_NUM_CHARS_PER_LINE);
    int yPos = y * _charSize.height;
    int xPos = x * _charSize.width;
    int textWidth = maxLength * _charSize.width;
    _lcd->clearArea(xPos, yPos, textWidth, _charSize.height);
    _lcd->writeTextBuffer(xPos, yPos, text.data(), maxLength, true);
}

void OledDisplay::clearLine(int lineNumber) {
    if (lineNumber < 0 || lineNumber >= OLED_NUM_TEXT_LINES) {
        return;
    }
    int yPos = (lineNumber * _charSize.height);
    _lcd->clearArea(0, yPos, _screenSize.width, _charSize.height);
}

void OledDisplay::drawRect(int x, int y, int width, int height, bool color, bool fill) {
    _lcd->rect(x, y, width, height, color, fill);
}

void OledDisplay::showMenu(const std::string &title, std::string *menuItems, int currentItem, int numMenuItems, int selectedItem) {
    clear(false);

    setTitle(title);

    int startView = currentItem - 1;
    int endView = currentItem + 1;

    if (endView >= numMenuItems) {
        endView = numMenuItems - 1;
        startView = numMenuItems - 3;
    }

    if (startView < 0) {
        startView = 0;
        endView = 2;
    }

    int currentLine = 1;
    for (int i = startView; i <= endView; i++) {
        if (i >= numMenuItems) {
            break;
        }

        std::string itemText = menuItems[i];
        if (selectedItem >= 0) {
            if (selectedItem == i) {
                itemText = "* " + itemText;
            } else {
                itemText = "  " + itemText;
            }
        }

        bool isCurrentItem = (i == currentItem);
        writeLineAt(currentLine, itemText, isCurrentItem);
        currentLine++;
    }

    show();
}
