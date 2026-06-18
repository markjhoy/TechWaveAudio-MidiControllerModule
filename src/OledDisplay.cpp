//
// Created by Mark J. Hoy on 2026-05-01.
//

#include "OledDisplay.h"

#include <cstring>

OledDisplay::OledDisplay(HardwareI2C *i2c) {
    _lcd = new Ssd1306(i2c, OLED_ADDRESS, OLED_DISPLAY_WIDTH, OLED_DISPLAY_HEIGHT);
    _displayTextBufferSize = OLED_NUM_TEXT_LINES * OLED_NUM_CHARS_PER_LINE;
    _displayTextBuffer = new char[_displayTextBufferSize];
    _highlightedLines = new bool[OLED_NUM_TEXT_LINES];
    memset(_displayTextBuffer, 0, _displayTextBufferSize);
}

OledDisplay::~OledDisplay() {
    delete[] _highlightedLines;
    delete[] _displayTextBuffer;
}

void OledDisplay::clear(bool refresh) {
    memset(_displayTextBuffer, 0, _displayTextBufferSize);
    for (int i = 0; i < OLED_NUM_TEXT_LINES; i++) {
        _highlightedLines[i] = false;
    }
    if (refresh) {
        _lcd->clear();
        _lcd->show();
    }
}

void OledDisplay::show(bool clearDisplay) {
    writeDisplayTextBuffer(clearDisplay);
}

void OledDisplay::setTitle(const std::string &title) {
    writeLineAt(0, title, true);
}

void OledDisplay::displayBootScreen() {
    clear(true);
    writeLines(boot_screen_lines, 4);
    show();
}

void OledDisplay::writeLines(char const *lines, int length) {
    int maxNumChars = std::min(length, _displayTextBufferSize);
    memcpy(_displayTextBuffer, lines, maxNumChars * sizeof(char));
    writeDisplayTextBuffer(true);
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

    int yPos = lineNumber * OLED_NUM_CHARS_PER_LINE;
    unsigned int lineLen = line.length();
    auto linePtr = line.data();
    for (int x = 0; x < OLED_NUM_CHARS_PER_LINE && x < lineLen; x++) {
        _displayTextBuffer[x + yPos] = (*linePtr);
        linePtr++;
    }
    _highlightedLines[lineNumber] = highlight;
}

void OledDisplay::writeTextAt(int x, int y, const std::string &text, bool writeDirect) {
    if (x < 0 || x > OLED_NUM_CHARS_PER_LINE || y < 0 || y > OLED_NUM_TEXT_LINES) {
        return;
    }

    int maxLength = std::min((int)text.length(), OLED_NUM_CHARS_PER_LINE - x);
    int textIndex = 0;
    int currentPos = y * OLED_NUM_CHARS_PER_LINE + x;
    int numCharsWritten = 0;
    for (int i = 0; i < text.length() && i < maxLength; i++) {
        _displayTextBuffer[currentPos++] = text[textIndex++];
        numCharsWritten++;
    }

    if (writeDirect) {
        char *bufferPos = _displayTextBuffer + (y * OLED_NUM_CHARS_PER_LINE + x);
        _lcd->writeTextBuffer(0, y * OLED_PIXELS_PER_LINE, bufferPos, numCharsWritten, true);
    }
}

void OledDisplay::clearLine(int lineNumber) {
    if (lineNumber < 0 || lineNumber >= OLED_NUM_TEXT_LINES) {
        return;
    }
    int yPos = (lineNumber * OLED_PIXELS_PER_LINE);
    _lcd->clearArea(0, yPos, OLED_DISPLAY_WIDTH, OLED_PIXELS_PER_LINE);
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

void OledDisplay::writeDisplayTextBuffer(bool clearLcd) {
    if (clearLcd) {
        _lcd->clear();
    }
    char *displayPos = _displayTextBuffer;
    for (int y = 0; y < OLED_NUM_TEXT_LINES; y++) {
        int yPos = y * OLED_PIXELS_PER_LINE;
        if (_highlightedLines[y] == true) {
            _lcd->rect(0, yPos, OLED_DISPLAY_WIDTH, OLED_PIXELS_PER_LINE, true, true);
            _lcd->writeTextBuffer(0, yPos, displayPos, OLED_NUM_CHARS_PER_LINE, false);
        } else {
            _lcd->writeTextBuffer(0, yPos, displayPos, OLED_NUM_CHARS_PER_LINE, true);
        }
        displayPos += OLED_NUM_CHARS_PER_LINE;
    }
    _lcd->show();
}
