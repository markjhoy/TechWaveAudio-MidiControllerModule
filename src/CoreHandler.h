//
// Created by Mark J. Hoy on 2026-06-07.
//

#ifndef TECHWAVEAUDIO_MCM_COREHANDLER_H
#define TECHWAVEAUDIO_MCM_COREHANDLER_H
#include "MultiCoreController.h"

/**
 * Base abstract core handler.
 * Provides functionality for processing incoming signals and sending messages to the other core.
 */
class CoreHandler {
 public:
    explicit CoreHandler(queue_t *inputQueue, queue_t *outputQueue) {
        _multiCoreController = new MultiCoreController(inputQueue, outputQueue);
    }

    virtual ~CoreHandler() {
        delete _multiCoreController;
    }

    /**
     * Processes any events waiting in the read queue.
     * For each event, the `processSignalMessage` function is called with that signal data.
     * @param maxEvents the max number of events to process before returning (default 0 = all events)
     */
    void processEvents(int maxEvents = 0);

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
     */
    virtual void processSignalMessage(SignalCommand command, uint8_t data) = 0;
private:
    MultiCoreController *_multiCoreController = nullptr;
};

#endif //TECHWAVEAUDIO_MCM_COREHANDLER_H
