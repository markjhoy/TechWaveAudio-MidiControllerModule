/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_IMAGESET_H
#define TECHWAVEAUDIO_MCM_IMAGESET_H
#include "../TechWaveAudio_MidiControllerModule.h"

/**
 * Base class for an set of images for the display
 */
class ImageSet {
public:
    ImageSet() = default;
    virtual ~ImageSet() = default;

    /**
     * Gets the total number of images in this set
     * @return The number of images in the set
     */
    virtual int getNumImages() = 0;

    /**
     * Get the image at the specified index
     * @param imageIndex the image index
     * @param outImageSize (output) the size of the image (width and height)
     * @param outImageNumBytes (output) the size of the image in bytes
     * @return pointer to the image byte data
     */
    virtual uint8_t *getImage(int imageIndex, BoxSize &outImageSize, int &outImageNumBytes) = 0;

    /**
     * Gets the first image in the set
     * @param outImageSize (output) the size of the image (width and height)
     * @param outImageNumBytes (output) the size of the image in bytes
     * @return pointer to the image byte data
     */
    uint8_t *getImage(BoxSize &outImageSize, int &outImageNumBytes) {
        return getImage(0, outImageSize, outImageNumBytes);
    }
};


#endif //TECHWAVEAUDIO_MCM_IMAGESET_H