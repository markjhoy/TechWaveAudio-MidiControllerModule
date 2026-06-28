
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "CharacterMapping8x16.h"

#include <cstring>

void CharacterMapping8x16::initialize() {
    memcpy(_characterSet, char_byte_mapping_8x16, _characterSetBufferSize);
    _initialized = true;
}
