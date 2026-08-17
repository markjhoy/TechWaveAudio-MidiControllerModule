
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H
#include <cstdint>

#define BASE_CHARACTER_SET_NUM_CHARS_DEFINED 0x60

/**
 * Base character set for an OLED display
 */
class BaseCharacterSet {
public:
    BaseCharacterSet(int characterWidth, int characterHeight);

    virtual ~BaseCharacterSet();

    /**
     * Gets the pixel width of a character
     * @return the width in pixels of a character
     */
    [[nodiscard]] inline int getCharacterWidth() const {return _characterWidth;}

    /**
     * Gets the pixel height of a character
     * @return the height in pixels of a character
     */
    [[nodiscard]] inline int getCharacterHeight() const {return _characterHeight;}

    /**
     * Gets the total number of bytes for a character
     * @return the total number of bytes for a character
     */
    [[nodiscard]] inline int getBytesPerChar() const {return _bytesPerChar;}

    /**
     * Gets the character width in number of bytes
     * @return the character width in bytes
     */
    [[nodiscard]] inline int getCharacterWidthInBytes() const { return _characterWidthBytes; }

    /**
     * Gets the entire character set data as a byte pointer
     * @return the character set data
     */
    [[nodiscard]] inline uint8_t *getCharacterArray() const {
        return _characterSet;
    }

protected:
    int _characterWidth;
    int _characterHeight;
    int _bytesPerChar;
    int _characterSetBufferSize;
    int _characterWidthBytes;
    uint8_t *_characterSet = nullptr;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H