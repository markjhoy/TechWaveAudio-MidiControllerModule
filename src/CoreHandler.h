
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_COREHANDLER_H
#define TECHWAVEAUDIO_MCM_COREHANDLER_H
#include "MultiCoreController.h"
#include "pico/flash.h"
#include "pico/util/queue.h"

/**
 * Base abstract core handler.
 * Provides functionality for processing incoming signals and sending messages to the other core.
 */
class CoreHandler {
 public:
    explicit CoreHandler(queue_t *inputQueue, queue_t *outputQueue) __attribute__((nonnull)) {
        _multiCoreController = new MultiCoreController(inputQueue, outputQueue);
    }

    virtual ~CoreHandler() {
        delete _multiCoreController;
    }

    void init() {
        flash_safe_execute_core_init();
    }

    /**
     * Processes up to MAX_MESSAGE_EVENTS_TO_PROCESS events in the read queue
     * @return true if at least one event was processed
     */
    bool processEvents();

protected:
    /**
     * Sends a message to the other core.
     * @param command the command to send
     * @param data data value to send
     */
    void sendSignal(SignalCommand command, uint8_t data = 0) const;

    /**
     * Gets the multicore controller for this handler
     * @return the underlying multicore controller
     */
    [[nodiscard]] MultiCoreController *getMultiCoreController() const { return _multiCoreController; }

    /**
     * Processes an incoming signal message from the `processEvents` function.
     * @param command the command that was received
     * @param data the data value received
     * @return true is a signal message was processed
     */
    virtual bool processSignalMessage(SignalCommand command, uint8_t data) = 0;

    /**
     * Allows for any functionality after events are processed
     */
    virtual void onAfterProcessEvents(bool messagesProcessed) = 0;
private:
    MultiCoreController *_multiCoreController = nullptr;
};

#endif //TECHWAVEAUDIO_MCM_COREHANDLER_H
