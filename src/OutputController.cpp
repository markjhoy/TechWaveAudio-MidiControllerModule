
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OutputController.h"

#include <cmath>

#include "MidiController.h"
#include "OutputRouteMap.h"
#include "TechWaveAudio_MidiControllerModule.h"
#include "hardware/gpio.h"

extern MidiController *global_midi_controller;

OutputController::OutputController(SystemState *systemState, TimedEventQueue *eventQueue) {
    _systemState = systemState;
    _eventQueue = eventQueue;
    setup();
}

OutputController::OutputController(SystemState *systemState, TimedEventQueue *eventQueue,
    MultiCoreController *multiCoreController) {
    _systemState = systemState;
    _eventQueue = eventQueue;
    _multiCoreController = multiCoreController;
    setup();
}

OutputController::~OutputController() {
    shutdown();

    delete _mappingRoute;
    delete _noteOutput;
    delete _ctlAuxDacOutput;
    delete _velocityOutput;
    delete _noteVelocityI2c;
}

void OutputController::init() {
    setupOutputPin(PIN_CLOCK_LINE);
    setupOutputPin(PIN_TRIGGER_LINE);
    setupOutputPin(PIN_GATE_LINE);
    setupOutputPin(PIN_NOTE_LED);
    setupOutputPin(PIN_CLOCK_LED);

    _noteOutput->write(0);
    _velocityOutput->write(0);
    _ctlAuxDacOutput->writeAux(0);
    _ctlAuxDacOutput->writeCtl(0);
    gpio_put(PIN_CLOCK_LINE, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);

    if (global_midi_controller == nullptr) {
        throw std::exception();
    }

    setPitchBendRangeChanged();

    global_midi_controller->stop();

    updateMappingRoutes();

    _sustainValue = false;

    global_midi_controller->setOnResetCallback([this] { onResetCallback(); });
    global_midi_controller->setOnClockCallback([this] { onClockCallback(); });
    global_midi_controller->setOnAllNotesOffCallback([this] { allNotesOffCallback(); });

    global_midi_controller->setOnModWheelCallback([this](auto && PH1) { onModWheelCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnSustainCallback([this](auto && PH1) { onSustainCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnVolumeChangedCallback([this](auto && PH1) { onVolumeCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnAftertouchCallback([this](auto && PH1) { onAftertouchCallback(std::forward<decltype(PH1)>(PH1)); });

    global_midi_controller->setOnNoteOnCallback([this](auto && PH1, auto && PH2) { noteOnCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    global_midi_controller->setOnNoteOffCallback([this](auto && PH1, auto && PH2) { noteOffCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    global_midi_controller->setOnPitchBendCallback([this](auto && PH1, auto && PH2) { onPitchBendCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });

    sleep_ms(500);

    global_midi_controller->start();

    _currentState.midiChannel = _systemState->midiChannel;
    _isRunning = true;
}

void OutputController::shutdown() {
    if (!_isRunning)
        return;
    global_midi_controller->stop();
    _isRunning = false;
}

void OutputController::updateMappingRoutes() {
    std::vector<OutputMappingRouteItem> newRoutes;

    newRoutes.push_back({_systemState->auxOutMapping, OutputMappingOutput_Aux});
    newRoutes.push_back({_systemState->ctlOutMapping, OutputMappingOutput_Control});
    newRoutes.push_back({_systemState->clockOutputMapping, OutputMappingOutput_Clock});

    // TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO --
    // TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO --
    // TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO --
    // TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO -- TODO --
    // reset any outputs only if outputs have changed
    // writeControlData(0);
    // writeAuxData(0);

    _mappingRoute->updateRoutes(newRoutes);
}

void OutputController::checkSendMapEntry(uint16_t mapping, uint8_t data, OutputMappingOutput output, const std::function<void(uint8_t)>& callback) {
    if ((mapping & output) == 0)
        return;

    callback(data);
}

void OutputController::outputMappedRoute(uint8_t data, OutputMappingRoute route, const MappedRouteCallback& callback) {
    auto mapping = _mappingRoute->getRouteMapping(route);

    if (mapping == 0)
        return;

    callback(data, mapping);
}

void OutputController::setupOutputPin(int pinId) {
    gpio_init(pinId);
    gpio_set_dir(pinId, GPIO_OUT);
}

void OutputController::setup() {
    _mappingRoute = new OutputRouteMap();
    _noteVelocityI2c = new HardwareI2C(&HW_DAC_4725_I2C, DAC_4725_I2C_DATA_PIN, DAC_4725_I2C_CLOCK_PIN, HW_DAC_4725_I2C_BAUD_RATE);
    _noteOutput = new Mcp4725(_noteVelocityI2c, DAC_NOTE_I2C_ADDRESS);
    _velocityOutput = new Mcp4725(_noteVelocityI2c, DAC_VELOCITY_I2C_ADDRESS);
    _ctlAuxDacOutput = new CtlAuxDacOutput(
        DAC_4902_SPI_BUS,
        DAC_4902_BAUD_RATE,
        DAC_4902_SPI_CLOCK_PIN,
        DAC_4902_SPI_TX_PIN,
        DAC_4902_SPI_RX_PIN,
        DAC_4902_SPI_CS_PIN
    );
}

void OutputController::sendCoreSignal(SignalCommand command, uint8_t data) const {
    if (_multiCoreController != nullptr) {
        _multiCoreController->sendSignalMessage({ command, data } );
    }
}

// ReSharper disable once CppDFAUnreachableFunctionCall
void OutputController::sendNoteWithBendAndAdjust(uint8_t midiNote) {
    // base note value
    float noteValue = 0.0;
    if (_systemState->noteCVMaxVoltage == FiveVoltOutput)
        // adjust to C2 = 0v
        noteValue = static_cast<float>(five_volt_note_12_bit_output[midiNote - MIDI_MIN_NOTE_5V]);
    else
        // adjust to C-1 = 0v
        noteValue = static_cast<float>(ten_volt_note_12_bit_output[midiNote - 12]);

    // +/- pitch bend
    noteValue += _currentPitchBend;

    // pitch adjustment
    noteValue += _systemState->pitchAdjust;

    int finalNoteValue = std::floor(noteValue);

    // clamp - just in case
    if (finalNoteValue < 1) {
        finalNoteValue = 1;
    } else if (noteValue >= DAC_4725_MAX_RANGE) {
        finalNoteValue = DAC_4725_MAX_RANGE - 1;
    }

    if (_systemState->noteCVMaxVoltage == FiveVoltOutput) {
        // half the value for +5v output
        finalNoteValue = finalNoteValue >> 1;
    }

    _noteOutput->write(finalNoteValue);
    _currentState.currentNote = midiNote;
    sendCoreSignal(SignalCommand_NoteChange, midiNote);
}

void OutputController::writeAuxData(uint8_t data) const {
    _ctlAuxDacOutput->writeAux(
        _systemState->auxCVMaxVoltage == TenVoltOutput ? static_cast<int>(data) << 1 : static_cast<int>(data)
    );
    sendCoreSignal(SignalCommand_AuxChange, data);
}

void OutputController::writeControlData(uint8_t data) const {
    _ctlAuxDacOutput->writeCtl(
        _systemState->ctlCVMaxVoltage == TenVoltOutput ? static_cast<int>(data) << 1 : static_cast<int>(data)
    );
    sendCoreSignal(SignalCommand_ControlChange, data);
}

void OutputController::noteOnCallback(uint8_t midiNoteNumber, uint8_t velocity) {
    if (_systemState->noteCVMaxVoltage == TenVoltOutput) {
        if (midiNoteNumber < MIDI_MIN_NOTE_10V || midiNoteNumber > MIDI_MAX_NOTE_10V)
            return;
    } else {
        if (midiNoteNumber < MIDI_MIN_NOTE_5V || midiNoteNumber > MIDI_MAX_NOTE_5V)
            return;
    }

    // if we have a current note, check the note priority
    if (_lastNote != DEFAULT_LAST_NOTE_VALUE) {
        if (_systemState->notePriority == NOTE_PRIORITY_HIGHEST && midiNoteNumber < _lastNote) {
            return;
        }
        if (_systemState->notePriority == NOTE_PRIORITY_LOWEST && midiNoteNumber > _lastNote) {
            return;
        }
        // if neither, we're using NOTE_PRIORITY_LAST
    }

    // remove any trigger callback (if we have one)
    _eventQueue->removeCallbackEvent(_lastTriggerQueueId);

    sendNoteWithBendAndAdjust(midiNoteNumber);

    uint16_t velocityValue = ten_volt_linear_12_bit_output[velocity];
    if (_systemState->velocityCVMaxVoltage == FiveVoltOutput) {
        velocityValue = velocityValue >> 1;
    }
    _velocityOutput->write(velocityValue);
    _currentState.currentVelocity = velocity;
    sendCoreSignal(SignalCommand_VelocityChange, velocity);

    gpio_put(PIN_NOTE_LED, true);
    gpio_put(PIN_TRIGGER_LINE, true);
    gpio_put(PIN_GATE_LINE, true);
    _currentState.triggerState = true;
    _currentState.gateState = true;

    sendCoreSignal(SignalCommand_TriggerPulse_On, 0);
    sendCoreSignal(SignalCommand_Gate_On, 0);

    // and add our timed callback to turn off the trigger line
    _lastTriggerQueueId = _eventQueue->scheduleCallbackEvent([this] {
        gpio_put(PIN_TRIGGER_LINE, false);
        sendCoreSignal(SignalCommand_TriggerPulse_Off, 0);
        _currentState.triggerState = false;
    }, _systemState->triggerDuration);

    _lastNote = midiNoteNumber;
}

void OutputController::noteOffCallback(uint8_t note, uint8_t _) {
    if (note != _lastNote && note != DEFAULT_LAST_NOTE_VALUE) {
        return;
    }
    if (_sustainValue && _lastNote != DEFAULT_LAST_NOTE_VALUE) {
        return;
    }
    allNotesOffCallback();
}

void OutputController::allNotesOffCallback() {
    _eventQueue->removeCallbackEvent(_lastTriggerQueueId);
    _noteOutput->write(0);
    _velocityOutput->write(0);
    _ctlAuxDacOutput->writeAux(0);
    _ctlAuxDacOutput->writeCtl(0);
    gpio_put(PIN_NOTE_LED, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);
    sendCoreSignal(SignalCommand_NoteChange, DEFAULT_LAST_NOTE_VALUE);
    sendCoreSignal(SignalCommand_VelocityChange, 0);
    sendCoreSignal(SignalCommand_TriggerPulse_Off, 0);
    sendCoreSignal(SignalCommand_Gate_Off, 0);
    _lastNote = DEFAULT_LAST_NOTE_VALUE;
    _lastTriggerQueueId = INVALID_EVENT_ID;
    _currentState = RunningState();
}

void OutputController::setPitchBendRangeChanged() {
    _lastPitchBendRangeValue = _systemState->pitchBendRange;
    if (_systemState->pitchBendRange == 0.0f) {
        _valuesPerSemitone = 0.0f;
        return;
    }
    _valuesPerSemitone = 8192.0f / (_systemState->pitchBendRange * 12.0f);
}

void OutputController::onPitchBendCallback(uint8_t fineValue, uint8_t coarseValue) {
    if (_systemState->pitchBendRange == 0.0f) {
        return;
    }

    // recalculate _valuesPerSemitone if the range changed
    if (_lastPitchBendRangeValue != _systemState->pitchBendRange) {
        setPitchBendRangeChanged();
    }

    // get our value -8192 to +8192
    // multiply coarse by 128 and add the fine value
    // then subtract our center (8192)
    int pitchBendValue =((static_cast<int>(coarseValue) << 7) & 0x3F80) + (fineValue & 0x007F) - 8192;

    // midpoint is 0, if we're +/- the middle tolerance, set pitch bend off
    if (pitchBendValue >= (-PITCH_BEND_ZERO_TOLERANCE) && pitchBendValue <= PITCH_BEND_ZERO_TOLERANCE) {
        // turn pitch bend off
        _currentPitchBend = 0.0f;
        if (_lastNote != DEFAULT_LAST_NOTE_VALUE) {
            sendNoteWithBendAndAdjust(_lastNote);
        }
        return;
    }

    // 12 steps per octave -> 12 steps per volt
    float bendInVolts = (static_cast<float>(pitchBendValue) / _valuesPerSemitone) / 12.0f;

    // our +/- 12 bit change value for 0 to 10v range (4096 / 10 -> 409.6)
    _currentPitchBend = bendInVolts * (409.6f);

    // and if our note is currently on, modify the output
    if (_lastNote != DEFAULT_LAST_NOTE_VALUE) {
        sendNoteWithBendAndAdjust(_lastNote);
    }
}

void OutputController::onSustainCallback(uint8_t data) {
    if (data >= 64) {
        // sustain on
        _sustainValue = true;
    } else {
        // sustain off
        _sustainValue = false;
        noteOffCallback(_lastNote, 0);
    }
}

void OutputController::onVolumeCallback(uint8_t velocity) {
    if (_lastNote == DEFAULT_LAST_NOTE_VALUE) {
        return;
    }
    uint16_t velocityValue = ten_volt_linear_12_bit_output[velocity];
    if (_systemState->velocityCVMaxVoltage == FiveVoltOutput) {
        velocityValue = velocityValue >> 1;
    }
    _velocityOutput->write(velocityValue);
    sendCoreSignal(SignalCommand_VelocityChange, velocity);

    _currentState.currentVelocity = velocity;
}

void OutputController::onAftertouchCallback(uint8_t data) {
    outputMappedRoute(data, OutputMappingRoute_Aftertouch, [this](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Aux, [this](uint8_t data)  {
            writeAuxData(data);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Control, [this](uint8_t data)  {
            writeControlData(data);
        });
    });
}

void OutputController::onExpressionCallback(uint8_t data) {
    outputMappedRoute(data, OutputMappingRoute_Expression, [this](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Aux, [this](uint8_t data)  {
            writeAuxData(data);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Control, [this](uint8_t data)  {
            writeControlData(data);
        });
    });
}

void OutputController::onModWheelCallback(uint8_t data) {
    outputMappedRoute(data, OutputMappingRoute_ModWheel, [this](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Aux, [this](uint8_t data)  {
            writeAuxData(data);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Control, [this](uint8_t data)  {
            writeControlData(data);
        });
    });
}

void OutputController::onEffectOneCallback(uint8_t data) {
    outputMappedRoute(data, OutputMappingRoute_Effect_1, [this](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Aux, [this](uint8_t data)  {
            writeAuxData(data);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Control, [this](uint8_t data)  {
            writeControlData(data);
        });
    });
}

void OutputController::onEffectTwoCallback(uint8_t data) {
    outputMappedRoute(data, OutputMappingRoute_Effect_2, [this](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Aux, [this](uint8_t data)  {
            writeAuxData(data);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Control, [this](uint8_t data)  {
            writeControlData(data);
        });
    });
}

void OutputController::onResetCallback() {
    // stop and clear any incoming messages
    global_midi_controller->stop();

    // remove any timed event items
    _eventQueue->removeCallbackEvent(_clockCallbackQueueId);
    _eventQueue->removeCallbackEvent(_lastTriggerQueueId);

    // turn off any note
    noteOffCallback(DEFAULT_LAST_NOTE_VALUE, 0);

    _ctlAuxDacOutput->writeAux(0);
    _ctlAuxDacOutput->writeCtl(0);

    gpio_put(PIN_CLOCK_LED, false);
    gpio_put(PIN_CLOCK_LINE, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);
    sendCoreSignal(SignalCommand_Reset, 0);

    _clockTickCount = 0;
    _currentState = RunningState();

    // restart processing midi messages
    global_midi_controller->start();
}

void OutputController::onClockCallback() {
    // TODO -- add clock mapped output
    // TODO -- add clock mapped output
    // TODO -- add clock mapped output
    // TODO -- add clock mapped output
    // TODO -- add clock mapped output

    _clockTickCount++;
    _eventQueue->removeCallbackEvent(_clockCallbackQueueId);

    if (_systemState->clockTickLedCycle == 0) {
        gpio_put(PIN_CLOCK_LED, false);
    } else if ((_systemState->clockTickLedCycle == 24 && _clockTickCount == 0) || (_clockTickCount % _systemState->clockTickLedCycle) == 0) {
        _clockLedValue = !_clockLedValue;
        gpio_put(PIN_CLOCK_LINE, _clockLedValue);
    }

    gpio_put(PIN_CLOCK_LED, _clockTickCount < _systemState->clockTickLedCycle);
    gpio_put(PIN_CLOCK_LINE, true);

    _clockCallbackQueueId = _eventQueue->scheduleCallbackEvent([this] {
        _currentState.clockState = false;
        gpio_put(PIN_CLOCK_LINE, false);
    }, CLOCK_PULSE_MS);

    sendCoreSignal(SignalCommand_ClockTick, 0);

    _currentState.clockState = true;
    if (_clockTickCount >= 24) {
        _clockTickCount = 0;
    }
}
