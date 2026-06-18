//
// Created by Mark J. Hoy on 2026-05-01.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H
#include <cstdint>

#include "../TechWaveAudio_MidiControllerModule.h"

#define BASE_CHARACTER_SET_NUM_CHARS_DEFINED 0x60

class BaseCharacterSet {
public:
    BaseCharacterSet(int characterWidth, int characterHeight);

    virtual ~BaseCharacterSet();

    [[nodiscard]] inline int getCharacterWidth() const {return _characterWidth;}
    [[nodiscard]] inline int getCharacterHeight() const {return _characterHeight;}
    [[nodiscard]] inline int getBytesPerChar() const {return _bytesPerChar;}

    [[nodiscard]] inline uint8_t *getCharacterArray() const {
        if (!_initialized) {
            ThrowError("initialize() has not been called");
        }
        return _characterSet;
    }

protected:
    int _characterWidth;
    int _characterHeight;
    int _bytesPerChar;
    int _characterSetBufferSize;
    uint8_t *_characterSet = nullptr;
    bool _initialized = false;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASECHARACTERSET_H