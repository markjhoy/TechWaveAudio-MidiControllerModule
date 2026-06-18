//
// Created by Mark J. Hoy on 2026-06-07.
//

#ifndef TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H
#define TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H
#include <atomic>

#include "TechWaveAudio_MidiControllerModule.h"
#include "pico/util/queue.h"

#define MAX_READ_EVENTS_HARD_LIMIT 4096

class MultiCoreController {
public:
    MultiCoreController(queue_t *inputQueue, queue_t *outputQueue) {
        _inputQueue = inputQueue;
        _outputQueue = outputQueue;
    }

    ~MultiCoreController() = default;

    bool getNextSignal(SignalMessage &message);

    void sendSignalMessage(const SignalMessage &message) const;
private:
    queue_t *_inputQueue;
    queue_t *_outputQueue;
};


#endif //TECHWAVEAUDIO_MCM_MULTICORECONTROLLER_H