
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTCONTROLLER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTCONTROLLER_H
#include "MultiCoreController.h"
#include "OutputRouteMap.h"
#include "TechWaveAudio_MidiControllerModule.h"
#include "SystemState.h"
#include "TimedEventQueue.h"
#include "hardware/CtlAuxDacOutput.h"
#include "hardware/Mcp4725.h"
#include "pico/sem.h"

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

    /**
     * Shuts down the controller and stops any midi proessing
     */
    void shutdown();

    /**
     * Gets the current state of the outputs for the dashboard display
     * @return the current dashboard state
     */
    inline RunningState_t *getCurrentState() { return &_currentState; }

    /**
     * gets the DAC for the note output
     * @return the DAC object for the note output 1v/oct CV
     */
    inline Mcp4725 *getNoteOutput() { return _noteOutput; }

    /**
     * gets the DAC for the velocity output
     * @return the DAC object for the velocity output CV
     */
    inline Mcp4725 *getVelocityOutput() { return _velocityOutput; }

    /**
     * gets the DAC object for the aux and control output
     * @return the DAC object for the aux and control CV outputs
     */
    inline CtlAuxDacOutput * getCtlAuxOutput() { return _ctlAuxDacOutput; }

    void updateMappingRoutes();

private:
    SystemState *_systemState = nullptr;
    TimedEventQueue * _eventQueue = nullptr;
    MultiCoreController *_multiCoreController = nullptr;
    HardwareI2C *_noteVelocityI2c = nullptr;
    Mcp4725 *_noteOutput = nullptr;
    Mcp4725 *_velocityOutput = nullptr;
    CtlAuxDacOutput *_ctlAuxDacOutput = nullptr;

    uint32_t _clockTickCount = 0;
    bool _clockLedValue = false;
    uint8_t _lastNote = DEFAULT_LAST_NOTE_VALUE;
    uint32_t _clockCallbackQueueId = INVALID_EVENT_ID;
    uint32_t _lastTriggerQueueId = INVALID_EVENT_ID;
    bool _sustainValue = false;

    float _lastPitchBendRangeValue = -123456.789f;
    float _valuesPerSemitone = 0.0f;

    RunningState_t _currentState;

    // our mapping from the input to bitmapped outputs
    OutputRouteMap *_mappingRoute = nullptr;
    OutputMappingRoute _lastAuxRoute{};
    OutputMappingRoute _lastControlRoute{};
    OutputMappingRoute _lastClockRoute{};

    uint32_t _auxOutputQueueId = INVALID_EVENT_ID;
    uint32_t _ctlOutputQueueId = INVALID_EVENT_ID;

    float _currentPitchBend = 0.0f;
    bool _isRunning = false;

    static void setupOutputPin(int pinId);
    void setup();
    void sendCoreSignal(SignalCommand command, uint8_t data) const;

    void sendNoteWithBendAndAdjust(uint8_t midiNote);

    void writeAuxData(uint data) const;
    void writeAuxDataSignal(bool signal) const;
    void writeControlData(uint data) const;
    void writeControlDataSignal(bool signal) const;

    void outputMappedRoute(uint16_t data, OutputMappingRoute route, const MappedRouteCallback& callback) const;
    static void checkSendMapEntry(uint16_t mapping, uint16_t data, OutputMappingOutput output, const MappedRouteDataCallback& callback);

    void routeCVEventFrom12Bit(OutputMappingRoute route, uint16_t data) const;
    void routeCVEvent(OutputMappingRoute route, uint8_t data) const;
    void routeSignalEvent(OutputMappingRoute route, bool value) const;
    void routePulseEvent(OutputMappingRoute route, long pulseDuration);

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