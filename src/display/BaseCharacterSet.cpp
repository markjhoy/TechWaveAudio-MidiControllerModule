
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "BaseCharacterSet.h"

#include <cstring>
#include "../TechWaveAudio_MidiControllerModule.h"

BaseCharacterSet::BaseCharacterSet(int characterWidth, int characterHeight) {
    // NOTE - the width must be a multiple of 8
    assert((characterWidth % 8) == 0);

    _characterWidth = characterWidth;
    _characterWidthBytes = (characterWidth >> 3);
    _characterHeight = characterHeight;
    _bytesPerChar = _characterWidthBytes * characterHeight;
    _characterSetBufferSize = _bytesPerChar * BASE_CHARACTER_SET_NUM_CHARS_DEFINED;
    _characterSet = new uint8_t[_characterSetBufferSize];
}

BaseCharacterSet::~BaseCharacterSet() {
    delete[] _characterSet;
}
