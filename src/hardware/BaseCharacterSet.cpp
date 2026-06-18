//
// Created by Mark J. Hoy on 2026-05-01.
//

#include "BaseCharacterSet.h"

#include <cstring>
#include <stdexcept>
#include "../TechWaveAudio_MidiControllerModule.h"

BaseCharacterSet::BaseCharacterSet(int characterWidth, int characterHeight) {
    _characterWidth = characterWidth;
    _characterHeight = characterHeight;
    _bytesPerChar = (characterWidth / 8) * characterHeight;
    _characterSetBufferSize = _bytesPerChar * BASE_CHARACTER_SET_NUM_CHARS_DEFINED;
    _characterSet = new uint8_t[_characterSetBufferSize];
}

BaseCharacterSet::~BaseCharacterSet() {
    delete[] _characterSet;
}
