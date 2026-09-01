
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OledDisplay.h"

#include <chrono>
#include <cmath>
#include <cstring>

#include "BootScreen_2_0_0.h"

OledDisplay::OledDisplay(HardwareI2C *i2c) {
    _lcd = new Ssd1306(i2c, OLED_ADDRESS, OLED_DISPLAY_WIDTH, OLED_DISPLAY_HEIGHT);
    _screenSize = _lcd->getScreenSize();
    _bootScreenImageSet = new BootScreen_2_0_0();
}

OledDisplay::~OledDisplay() {
    delete _lcd;
    delete _bootScreenImageSet;
}

void OledDisplay::clear(bool refresh) {
    _lcd->clear();
    if (refresh) {
        _lcd->show();
    }
}

void OledDisplay::clearTextArea(int x, int y, int numChars, OledFontType font) {
    auto fontSize = _lcd->getTextCharacterSizing(font);
    int xPos = x * fontSize.width;
    int yPos = y * fontSize.height;
    int textWidth = numChars * fontSize.width;
    _lcd->clearArea(xPos, yPos, textWidth, fontSize.height);
}

void OledDisplay::clearLine(int lineNumber, OledFontType font) {
    if (lineNumber < 0 || lineNumber >= OLED_MAX_NUM_TEXT_LINES) {
        return;
    }
    auto charSize = _lcd->getTextCharacterSizing(font);
    int yPos = (lineNumber * charSize.height);
    _lcd->clearArea(0, yPos, _screenSize.width, charSize.height);
}

void OledDisplay::clearArea(int x, int y, int width, int height) {
    _lcd->clearArea(x, y, width, height);
}

void OledDisplay::show(bool clearDisplay) {
    _lcd->show();
}

void OledDisplay::setTitle(const std::string &title) {
    writeLineAt(0, title, false);
}

void OledDisplay::displayBootScreen() {
    clear(true);
    BoxSize imageSize;
    int byteCount;
    if (auto bootScreenImage = _bootScreenImageSet->getImage(imageSize, byteCount)) {
        _lcd->writeFullScreenBitmap(bootScreenImage, byteCount);
    }
    show();
}

void OledDisplay::blitImage(const int x, const int y, const uint8_t width, const uint8_t *data, const uint16_t numDataBytes) const {
    int bytesPerRow = std::ceil(static_cast<double>(width) / 8.0);
    int currentIndex = 0;
    int yPos = y;
    while (currentIndex < numDataBytes && yPos < OLED_DISPLAY_HEIGHT) {
        int currentBit = 0;
        int xPos = x;
        int lineStartIndex = currentIndex;
        for (int i = 0; i < width && currentIndex < numDataBytes && xPos < OLED_DISPLAY_WIDTH; i++) {
            bool color = (data[currentIndex] & (1 << currentBit)) > 0;
            _lcd->setPixel(xPos++, yPos, color);
            currentBit++;
            if (currentBit == 8) {
                currentBit = 0;
                currentIndex++;
            }
        }
        currentIndex = lineStartIndex + bytesPerRow;
        yPos++;
    }
}

void OledDisplay::writeLines(char const *lines, int length, bool highlightFirstLine, OledFontType font) {
    auto charSize = _lcd->getTextCharacterSizing(font);

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
        yPos += charSize.height;
        currentOffset += numCharsThisLine;
    }
    show();
}

void OledDisplay::writeLines(const std::string *lines, int numLines, OledFontType font) {
    for (int i = 0; i < numLines && i < OLED_MAX_NUM_TEXT_LINES; i++) {
        writeLineAt(i, lines[i], i==0);
    }
}

void OledDisplay::writeLines(const std::string &title, const std::string *lines, int numLines, OledFontType font) {
    setTitle(title);
    for (int i = 1; i < numLines; i++) {
        writeLineAt(i, lines[i], font);
    }
}

void OledDisplay::writeLineAt(int lineNumber, const std::string &line, bool highlight, OledFontType font) {
    if (lineNumber < 0 || lineNumber >= OLED_MAX_NUM_TEXT_LINES) {
        return;
    }

    auto charSize = _lcd->getTextCharacterSizing(font);
    int yPos = lineNumber * charSize.height;
    int lineLen = std::min(static_cast<int>(line.length()), OLED_NUM_CHARS_PER_LINE);
    auto linePtr = line.data();
    _lcd->clearArea(0, yPos, _screenSize.width, charSize.height);
    if (highlight) {
        _lcd->rect(0, yPos, _screenSize.width, charSize.height, true, true);
    }
    _lcd->writeTextBuffer(0, yPos, linePtr, lineLen, !highlight, font);
}

void OledDisplay::writeTextAt(int x, int y, const std::string &text, const OledFontType font, bool highlight) {
    if (x < 0 || x > OLED_DISPLAY_WIDTH || y < 0 || y > OLED_DISPLAY_HEIGHT) {
        return;
    }

    auto charSize = _lcd->getTextCharacterSizing(font);
    int textWidth = std::min((int)text.length() * charSize.width, OLED_DISPLAY_WIDTH);
    if (highlight) {
        _lcd->rect(x, y, textWidth, charSize.height, true, true);
    } else {
        _lcd->clearArea(x, y, textWidth, charSize.height);
    }
    _lcd->writeTextBuffer(x, y, text.data(), static_cast<int>(text.length()), !highlight, font);
}

void OledDisplay::writeTextStartingAtLine(int lineNumber, const std::string &text, OledFontType font) {
    int pos = 0;
    int currentLine = lineNumber;
    while (pos < text.length() && currentLine < OLED_MAX_NUM_TEXT_LINES) {
        auto thisBlock = text.substr(pos, 16);
        writeLineAt(currentLine, thisBlock, font);
        pos += 16;
        currentLine++;
    }
}

void OledDisplay::drawRect(int x, int y, int width, int height, bool color, bool fill) {
    _lcd->rect(x, y, width, height, color, fill);
}

void OledDisplay::showMenu(const std::string &title, std::string *menuItems, int currentItem, int numMenuItems, int selectedItem) {
    clear(false);

    setTitle(title);

    int startView = 0;
    int endView = numMenuItems;
    if (numMenuItems > MENU_SYSTEM_NUM_LINES) {
        int centerPoint = (MENU_SYSTEM_NUM_LINES >> 1);
        int numAbove = centerPoint;
        int numBelow = MENU_SYSTEM_NUM_LINES - numAbove;
        startView = currentItem - numAbove;
        endView = std::max(currentItem + numBelow, MENU_SYSTEM_NUM_LINES);
        if (startView < 0)
            startView = 0;
        if (endView > numMenuItems)
            endView = numMenuItems;
    }

    int currentLine = OLED_MENU_LINE_START;
    int i = startView;
    for (; currentLine < (MENU_SYSTEM_NUM_LINES + OLED_MENU_LINE_START) && i < endView && i < numMenuItems; i++) {
        std::string itemText = menuItems[i];
        if (selectedItem >= 0 && i > 0) {
            if (selectedItem == i) {
                itemText = "* " + itemText;
            } else {
                itemText = "  " + itemText;
            }
        }

        bool isCurrentItem = (i == currentItem);
        writeLineAt(currentLine, itemText, isCurrentItem, OLED_MENU_ITEM_FONT);
        currentLine++;
    }

    show();
}

void OledDisplay::setBrightness(const uint8_t value) const {
    uint valueToUse = value * 16;
    if (valueToUse > 255)
        valueToUse = 255;
    if (valueToUse <= 0)
        valueToUse = 1;
    this->_lcd->setContrast(valueToUse);
}
