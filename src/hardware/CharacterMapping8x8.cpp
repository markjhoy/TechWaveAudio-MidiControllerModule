//
// Created by Mark J. Hoy on 2026-07-27.
//

#include "CharacterMapping8x8.h"

#include <cstring>

CharacterMapping8x8::CharacterMapping8x8() : BaseCharacterSet(8, 8) {
    memcpy(_characterSet, char_byte_mapping_8x8, _characterSetBufferSize);
}
