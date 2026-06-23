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

    [[nodiscard]] bool shouldKeepRunning() const { return _keepRunning; }

protected:
    void processSignalMessage(SignalCommand command, uint8_t data) override;
    void onAfterProcessEvents() override;

private:
    volatile bool _keepRunning = true;
    OutputController *_outputController = nullptr;
    TimedEventQueue *_eventQueue = nullptr;
    SystemState *_systemState = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_MIDIANDOUTPUTHANDLER_H