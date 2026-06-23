//
// Created by Mark J. Hoy on 2026-05-01.
//

#include "CharacterMapping8x16.h"

#include <cstring>

void CharacterMapping8x16::initialize() {
    memcpy(_characterSet, char_byte_mapping_8x16, _characterSetBufferSize);
    _initialized = true;
}
