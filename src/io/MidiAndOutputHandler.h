
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H
#define TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H
#include "../CoreHandler.h"
#include "OutputController.h"
#include "pico/util/queue.h"

/**
 * Our core 1 handler. Runs MIDI input and hardware output functionality.
 */
class MidiAndOutputHandler : public CoreHandler {
public:
    MidiAndOutputHandler(queue_t *inputQueue, queue_t *outputQueue) __attribute__((nonnull));;
    ~MidiAndOutputHandler() override;

    /**
     * Returns the flag to keep running or not.
     * This will only go to false if a `SignalCommand_Shutdown` message is received
     * @return true if we should keep running
     */
    [[nodiscard]] bool shouldKeepRunning() const { return _keepRunning; }

protected:
    bool processSignalMessage(SignalCommand command, uint8_t data) override;
    void onAfterProcessEvents(bool messagesProcessed) override;

private:
    volatile bool _keepRunning = true;
    OutputController *_outputController = nullptr;
    TimedEventQueue *_eventQueue = nullptr;
    SystemState *_systemState = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H