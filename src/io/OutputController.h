
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTCONTROLLER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTCONTROLLER_H

#include "OutputRouteMap.h"
#include "../MultiCoreController.h"
#include "../TechWaveAudio_MidiControllerModule.h"
#include "../SystemState.h"
#include "../common/TimedEventQueue.h"
#include "../hardware/NoteVelOut1Out2Output.h"
#include "pico/sem.h"

typedef struct NoteOnMapping_t {
    uint8_t note = DEFAULT_LAST_NOTE_VALUE;
    uint8_t velocity = 0;
    NoteOnMapping_t *next = nullptr;
    NoteOnMapping_t *previous = nullptr;
} NoteOnMapping;

/**
 * Our output controller.
 * Responsible for sending data out to our DACs and signal lines.
 */
class OutputController {
public:
    explicit OutputController(SystemState *systemState, TimedEventQueue *eventQueue);
    OutputController(SystemState *systemState, TimedEventQueue *eventQueue, MultiCoreController *multiCoreController);
    ~OutputController();

    /**
     * Initializes the controller and turns on midi and output processing
     */
    void init();

    void reset();

    /**
     * Shuts down the controller and stops any midi proessing
     */
    void shutdown();

    /**
     * Gets the current state of the outputs for the dashboard display
     * @return the current dashboard state
     */
    RunningState_t *getCurrentState() { return &_currentState; }

    [[nodiscard]] NoteVelOut1Out2Output * getNoteVelOut1Out2Output() const { return _noteVelOut1Out2Output; }

    [[nodiscard]] Dac7554 * getExtensionOutput() const { return _extensionOutput; }

    void updateMappingRoutes();

    void setIgnoreMidi(const bool value) { _ignoreMidi = value; }

    void processMidiQueue();

private:
    SystemState *_systemState = nullptr;
    TimedEventQueue * _eventQueue = nullptr;
    MultiCoreController *_multiCoreController = nullptr;
    NoteVelOut1Out2Output *_noteVelOut1Out2Output = nullptr;
    Dac7554 *_extensionOutput = nullptr;

    uint32_t _clockTickCount = 0;
    bool _clockLedValue = false;
    uint8_t _lastNote = DEFAULT_LAST_NOTE_VALUE;
    uint32_t _clockCallbackQueueId = INVALID_EVENT_ID;
    uint32_t _lastTriggerQueueId = INVALID_EVENT_ID;
    bool _sustainValue = false;
    bool _ignoreMidi = false;

    NoteOnMapping *_noteStack = nullptr;
    semaphore_t _noteQueueSemaphore{};

    float _lastPitchBendRangeValue = -123456.789f;
    float _valuesPerSemitone = 0.0f;

    RunningState_t _currentState;

    // our mapping from the input to bitmapped outputs
    OutputRouteMap *_mappingRoute = nullptr;
    OutputMappingRoute _lastOut1Route{};
    OutputMappingRoute _lastOut2Route{};
    OutputMappingRoute _lastClockRoute{};
    OutputMappingRoute _lastOutX1Route{};
    OutputMappingRoute _lastOutX2Route{};
    OutputMappingRoute _lastOutX3Route{};
    OutputMappingRoute _lastOutX4Route{};

    uint32_t _out1OutputQueueId = INVALID_EVENT_ID;
    uint32_t _out2OutputQueueId = INVALID_EVENT_ID;
    uint32_t _outX1OutputQueueId = INVALID_EVENT_ID;
    uint32_t _outX2OutputQueueId = INVALID_EVENT_ID;
    uint32_t _outX3OutputQueueId = INVALID_EVENT_ID;
    uint32_t _outX4OutputQueueId = INVALID_EVENT_ID;

    float _currentPitchBend = 0.0f;
    bool _isRunning = false;

    static void setupOutputPin(int pinId);
    void setupHwOutputs();
    void sendCoreSignal(SignalCommand command, uint8_t data) const;

    void sendNoteWithBendAndAdjust(uint8_t midiNote);

    void writeOut1Data(uint data) const;
    void writeOut1Signal(bool signal) const;
    void writeOut2Data(uint data) const;
    void writeOut2Signal(bool signal) const;
    void writeOutX1Data(uint data) const;
    void writeOutX1Signal(bool signal) const;
    void writeOutX2Data(uint data) const;
    void writeOutX2Signal(bool signal) const;
    void writeOutX3Data(uint data) const;
    void writeOutX3Signal(bool signal) const;
    void writeOutX4Data(uint data) const;
    void writeOutX4Signal(bool signal) const;

    void outputMappedRoute(uint16_t data, OutputMappingRoute route, const MappedRouteCallback& callback) const;
    static void checkSendMapEntry(uint16_t mapping, uint16_t data, OutputMappingOutput output, const MappedRouteDataCallback& callback);

    void routeCVEvent(OutputMappingRoute route, uint16_t value) const;
    void routeSignalEvent(OutputMappingRoute route, bool value) const;
    void routePulseEvent(OutputMappingRoute route, long pulseDuration);

    void pushOnCurrentNoteStack(uint8_t note, uint8_t velocity);
    NoteOnMapping *removeFromCurrentNoteStack(uint8_t note);
    void clearNoteQueue();

    // -- event callbacks --
    void noteOnCallback(uint8_t midiNoteNumber, uint8_t velocity);

    void noteOffCallback(uint8_t note, uint8_t _);
    void allNotesOffCallback();
    void onModWheelCallback(uint8_t data);

    void setPitchBendRangeChanged();
    void onPitchBendCallback(uint8_t fineValue, uint8_t coarseValue);

    void onSustainCallback(uint8_t data);
    void onVolumeCallback(uint8_t velocity);
    void onAftertouchCallback(uint8_t data);
    void onExpressionCallback(uint8_t data);
    void onEffectOneCallback(uint8_t data);
    void onEffectTwoCallback(uint8_t data);

    void onClockCallback();

    void onResetCallback();
    void onStartCallback();
    void onStopCallback();
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTCONTROLLER_H