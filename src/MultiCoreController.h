/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H
#define TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H

#include "TechWaveAudio_MidiControllerModule.h"
#include "pico/util/queue.h"

#define MAX_READ_EVENTS_HARD_LIMIT 4096

/**
 * A helper class for sending and receiving signal messages for the cross-core queues
 */
class MultiCoreController {
public:
    MultiCoreController(queue_t *inputQueue, queue_t *outputQueue) {
        _inputQueue = inputQueue;
        _outputQueue = outputQueue;
    }

    ~MultiCoreController() = default;

    /**
     * Checks and receives a signal message from the read (input) queue.
     * @param message the signal message received
     * @return true is a signal message was in the queue, false if not
     */
    bool getNextSignal(SignalMessage &message) const;

    /**
     * Sends a signal message to the output queue
     * @param message the message to send.
     */
    void sendSignalMessage(const SignalMessage &message) const;
private:
    queue_t *_inputQueue;
    queue_t *_outputQueue;
};


#endif //TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H