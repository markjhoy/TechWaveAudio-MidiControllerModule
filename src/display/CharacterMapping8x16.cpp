
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "CharacterMapping8x16.h"

#include <cstring>

CharacterMapping8x16::CharacterMapping8x16() : BaseCharacterSet(8, 16) {
    memcpy(_characterSet, char_byte_mapping_8x16, _characterSetBufferSize);
}
