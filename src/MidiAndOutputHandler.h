//
// Created by Mark J. Hoy on 2026-06-07.
//

#ifndef TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H
#define TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H
#include "CoreHandler.h"
#include "MultiCoreController.h"
#include "OutputController.h"


class MidiAndOutputHandler : public CoreHandler {
public:
    explicit MidiAndOutputHandler(queue_t *inputQueue, queue_t *outputQueue);
    ~MidiAndOutputHandler() override;

    bool shouldKeepRunning() { return _keepRunning; }

private:
    std::atomic<bool> _keepRunning = true;
    OutputController *_outputController = nullptr;
    TimedEventQueue *_eventQueue = nullptr;
    SystemState *_systemState = nullptr;

    void processSignalMessage(SignalCommand command, uint8_t data) override;
};


#endif //TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H