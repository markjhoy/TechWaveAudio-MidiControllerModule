/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_IMAGESET_H
#define TECHWAVEAUDIO_MCM_IMAGESET_H
#include "../TechWaveAudio_MidiControllerModule.h"

class ImageSet {
public:
    ImageSet() = default;
    virtual ~ImageSet() = default;

    virtual int getNumImages() = 0;

    virtual uint8_t *getImage(int imageIndex, BoxSize &outImageSize, int &outImageNumBytes) = 0;

    uint8_t *getImage(BoxSize &outImageSize, int &outImageNumBytes) {
        return getImage(0, outImageSize, outImageNumBytes);
    }
};


#endif //TECHWAVEAUDIO_MCM_IMAGESET_H