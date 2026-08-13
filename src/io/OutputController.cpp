
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
#include "../TechWaveAudio_MidiControllerModule.h"
#include "hardware/gpio.h"

extern MidiController *global_midi_controller;

OutputController::OutputController(SystemState *systemState, TimedEventQueue *eventQueue) {
    _systemState = systemState;
    _eventQueue = eventQueue;
    setupHwOutputs();
}

OutputController::OutputController(SystemState *systemState, TimedEventQueue *eventQueue,
    MultiCoreController *multiCoreController) {
    _systemState = systemState;
    _eventQueue = eventQueue;
    _multiCoreController = multiCoreController;
    setupHwOutputs();
}

OutputController::~OutputController() {
    shutdown();
    delete _noteVelOut1Out2Output;
    delete _extensionOutput;
}

void OutputController::init() {
    if (!_isRunning)
        sem_init(&_noteQueueSemaphore, 1, 1);
    else
        sem_reset(&_noteQueueSemaphore, 1);

    _ignoreMidi = true;

    setupOutputPin(PIN_CLOCK_LINE);
    setupOutputPin(PIN_TRIGGER_LINE);
    setupOutputPin(PIN_GATE_LINE);
    setupOutputPin(PIN_NOTE_LED);
    setupOutputPin(PIN_CLOCK_LED);

    _noteVelOut1Out2Output->writeNote(0);
    _noteVelOut1Out2Output->writeVelocity(0);
    _noteVelOut1Out2Output->writeOut1(0);
    _noteVelOut1Out2Output->writeOut2(0);
    gpio_put(PIN_CLOCK_LINE, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);

    if (_systemState->expansionSensed) {
        _extensionOutput->writeValue(EXT_OUT_X1_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X2_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X3_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X4_REGISTER, 0);
    }

    if (global_midi_controller == nullptr) {
        throw std::exception();
    }

    setPitchBendRangeChanged();

    global_midi_controller->stop();

    _mappingRoute = new OutputRouteMap();
    updateMappingRoutes();

    _sustainValue = false;

    global_midi_controller->setOnResetCallback([this] { onResetCallback(); });
    global_midi_controller->setOnClockCallback([this] { onClockCallback(); });
    global_midi_controller->setOnAllNotesOffCallback([this] { allNotesOffCallback(); });
    global_midi_controller->setOnStartCallback([this] { onStartCallback(); });
    global_midi_controller->setOnStopCallback([this] { onStopCallback(); });

    global_midi_controller->setOnModWheelCallback([this](auto && PH1) { onModWheelCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnSustainCallback([this](auto && PH1) { onSustainCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnVolumeChangedCallback([this](auto && PH1) { onVolumeCallback(std::forward<decltype(PH1)>(PH1)); });
    global_midi_controller->setOnAftertouchCallback([this](auto && PH1) { onAftertouchCallback(std::forward<decltype(PH1)>(PH1)); });

    global_midi_controller->setOnNoteOnCallback([this](auto && PH1, auto && PH2) { noteOnCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    global_midi_controller->setOnNoteOffCallback([this](auto && PH1, auto && PH2) { noteOffCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    global_midi_controller->setOnPitchBendCallback([this](auto && PH1, auto && PH2) { onPitchBendCallback(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });

    sleep_ms(500);

    _ignoreMidi = false;
    global_midi_controller->start();

    _currentState.midiChannel = _systemState->midiChannel;
    _isRunning = true;
}

void OutputController::shutdown() {
    if (!_isRunning)
        return;

    _isRunning = false;

    global_midi_controller->stop();

    clearNoteQueue();

    _noteVelOut1Out2Output->writeNote(0);
    _noteVelOut1Out2Output->writeVelocity(0);
    _noteVelOut1Out2Output->writeOut1(0);
    _noteVelOut1Out2Output->writeOut2(0);
    gpio_put(PIN_CLOCK_LINE, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);

    if (_systemState->expansionSensed) {
        _extensionOutput->writeValue(EXT_OUT_X1_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X2_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X3_REGISTER, 0);
        _extensionOutput->writeValue(EXT_OUT_X4_REGISTER, 0);
    }

    delete _mappingRoute;
    _mappingRoute = nullptr;
    sem_reset(&_noteQueueSemaphore, 1);
}

void OutputController::updateMappingRoutes() {
    // clear any existing event callbacks for outputs
    _eventQueue->removeCallbackEvent(_out2OutputQueueId);
    _eventQueue->removeCallbackEvent(_out1OutputQueueId);
    _eventQueue->removeCallbackEvent(_clockCallbackQueueId);

    std::vector<OutputMappingRouteItem> newRoutes;

    newRoutes.push_back({_systemState->out1Mapping, OutputMappingOutput_Out1});
    newRoutes.push_back({_systemState->out2Mapping, OutputMappingOutput_Out2});
    newRoutes.push_back({_systemState->clockOutputMapping, OutputMappingOutput_Clock});
    if (_systemState->expansionSensed) {
        newRoutes.push_back({_systemState->outX1Mapping, OutputMappingOutput_OutX1});
        newRoutes.push_back({_systemState->outX2Mapping, OutputMappingOutput_OutX2});
        newRoutes.push_back({_systemState->outX3Mapping, OutputMappingOutput_OutX3});
        newRoutes.push_back({_systemState->outX4Mapping, OutputMappingOutput_OutX4});
    }

    if (_systemState->out1Mapping != _lastOut1Route)
        writeOut1Data(0);
    if (_systemState->out2Mapping != _lastOut2Route)
        writeOut2Data(0);
    if (_systemState->clockOutputMapping != _lastClockRoute) {
        gpio_put(PIN_CLOCK_LINE, false);
        gpio_put(PIN_CLOCK_LED, false);
    }

    _lastOut1Route = _systemState->out1Mapping;
    _lastOut2Route = _systemState->out2Mapping;
    _lastClockRoute = _systemState->clockOutputMapping;

    if (_systemState->expansionSensed) {
        if (_systemState->outX1Mapping != _lastOutX1Route)
            writeOutX1Data(0);
        if (_systemState->outX2Mapping != _lastOutX2Route)
            writeOutX2Data(0);
        if (_systemState->outX3Mapping != _lastOutX3Route)
            writeOutX3Data(0);
        if (_systemState->outX4Mapping != _lastOutX4Route)
            writeOutX4Data(0);

        _lastOutX1Route = _systemState->outX1Mapping;
        _lastOutX2Route = _systemState->outX2Mapping;
        _lastOutX3Route = _systemState->outX3Mapping;
        _lastOutX4Route = _systemState->outX4Mapping;
    }

    _mappingRoute->updateRoutes(newRoutes);
}

void OutputController::setupOutputPin(int pinId) {
    gpio_init(pinId);
    gpio_set_dir(pinId, GPIO_OUT);
}

void OutputController::setupHwOutputs() {
    _noteVelOut1Out2Output = new NoteVelOut1Out2Output(
        MAIN_DAC_7554_SPI_BUS,
        MAIN_DAC_7554_BAUD_RATE,
        DAC_7554_SPI_CLOCK_PIN,
        DAC_7554_SPI_TX_PIN,
        DAC_7554_SPI_RX_PIN,
        DAC_7554_SPI_CS_PIN
    );

    if (_systemState -> expansionSensed) {
        _extensionOutput = new Dac7554(
            EX_DAC_7554_SPI_BUS,
            EX_DAC_7554_BAUD_RATE,
            PIN_EX_SPI_CLOCK,
            PIN_EX_SPI_TX,
            PIN_EX_SPI_RX,
            PIN_EX_SPI_CS
        );
    }
}

void OutputController::sendCoreSignal(SignalCommand command, uint8_t data) const {
    if (_multiCoreController != nullptr) {
        _multiCoreController->sendSignalMessage({ command, data } );
    }
}

void OutputController::writeOut1Data(uint data) const {
    _noteVelOut1Out2Output->writeOut1(
        _systemState->out1CVMaxVoltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_Out1Change, data);
}

void OutputController::writeOut1Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 1) : 0;
    _noteVelOut1Out2Output->writeOut1(dataValue);
    sendCoreSignal(SignalCommand_Out1Change, signal ? 255 : 0);
}

void OutputController::writeOut2Data(uint data) const {
    _noteVelOut1Out2Output->writeOut2(
        _systemState->out2CVMaxVoltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_Out2Change, data);
}

void OutputController::writeOut2Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 2) : 0;
    _noteVelOut1Out2Output->writeOut2(dataValue);
    sendCoreSignal(SignalCommand_Out2Change, signal ? 255 : 0);
}

void OutputController::writeOutX1Data(uint data) const {
    _extensionOutput->writeValue(EXT_OUT_X1_REGISTER,
        _systemState->outX1Voltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_OutX1Change, data);
}

void OutputController::writeOutX1Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 1) : 0;
    _extensionOutput->writeValue(EXT_OUT_X1_REGISTER, dataValue);
    sendCoreSignal(SignalCommand_OutX1Change, signal ? 255 : 0);
}

void OutputController::writeOutX2Data(uint data) const {
    _extensionOutput->writeValue(EXT_OUT_X2_REGISTER,
        _systemState->outX2Voltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_OutX2Change, data);
}

void OutputController::writeOutX2Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 1) : 0;
    _extensionOutput->writeValue(EXT_OUT_X2_REGISTER, dataValue);
    sendCoreSignal(SignalCommand_OutX2Change, signal ? 255 : 0);
}

void OutputController::writeOutX3Data(uint data) const {
    _extensionOutput->writeValue(EXT_OUT_X3_REGISTER,
        _systemState->outX3Voltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_OutX3Change, data);
}

void OutputController::writeOutX3Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 1) : 0;
    _extensionOutput->writeValue(EXT_OUT_X3_REGISTER, dataValue);
    sendCoreSignal(SignalCommand_OutX3Change, signal ? 255 : 0);
}

void OutputController::writeOutX4Data(uint data) const {
    _extensionOutput->writeValue(EXT_OUT_X4_REGISTER,
        _systemState->outX4Voltage == TenVoltOutput ? static_cast<int>(data) : static_cast<int>(data >> 1)
    );
    sendCoreSignal(SignalCommand_OutX4Change, data);
}

void OutputController::writeOutX4Signal(bool signal) const {
    uint16_t dataValue = signal ? (DAC_7554_MAX_RANGE >> 1) : 0;
    _extensionOutput->writeValue(EXT_OUT_X4_REGISTER, dataValue);
    sendCoreSignal(SignalCommand_OutX4Change, signal ? 255 : 0);
}

void OutputController::outputMappedRoute(uint16_t data, OutputMappingRoute route, const MappedRouteCallback& callback) const {
    auto mapping = _mappingRoute->getRouteMapping(route);

    if (mapping == 0)
        return;

    callback(data, mapping);
}

void OutputController::checkSendMapEntry(uint16_t mapping, uint16_t data, OutputMappingOutput output, const std::function<void(uint16_t)>& callback) {
    if ((mapping & output) == 0)
        return;

    callback(data);
}

void OutputController::routeCVEvent(OutputMappingRoute route, uint16_t value) const {
    outputMappedRoute(value, route, [this](uint8_t value, uint16_t mapping) {
        checkSendMapEntry(mapping, value, OutputMappingOutput_Out1, [this](uint16_t value)  {
            writeOut1Data(value);
        });
        checkSendMapEntry(mapping, value, OutputMappingOutput_Out2, [this](uint16_t value)  {
            writeOut2Data(value);
        });

        if (_systemState->expansionSensed) {
            checkSendMapEntry(mapping, value, OutputMappingOutput_OutX1, [this](uint16_t value)  {
                writeOutX1Data(value);
            });
            checkSendMapEntry(mapping, value, OutputMappingOutput_OutX2, [this](uint16_t value)  {
                writeOutX2Data(value);
            });
            checkSendMapEntry(mapping, value, OutputMappingOutput_OutX3, [this](uint16_t value)  {
                writeOutX3Data(value);
            });
            checkSendMapEntry(mapping, value, OutputMappingOutput_OutX4, [this](uint16_t value)  {
                writeOutX4Data(value);
            });
        }
    });
}

void OutputController::routeSignalEvent(OutputMappingRoute route, bool value) const {
    outputMappedRoute(0, route, [this, value](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Out1, [this, value](uint16_t data)  {
            writeOut1Signal(value);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Out2, [this, value](uint16_t data)  {
            writeOut2Signal(value);
        });

        if (_systemState->expansionSensed) {
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX1, [this, value](uint16_t data)  {
                writeOutX1Signal(value);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX2, [this, value](uint16_t data)  {
                writeOutX2Signal(value);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX3, [this, value](uint16_t data)  {
                writeOutX3Signal(value);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX4, [this, value](uint16_t data)  {
                writeOutX4Signal(value);
            });
        }
    });
}

void OutputController::routePulseEvent(OutputMappingRoute route, long pulseDuration) {
    outputMappedRoute(0, route, [this, pulseDuration](uint8_t data, uint16_t mapping) {
        checkSendMapEntry(mapping, data, OutputMappingOutput_Clock, [this](uint16_t data) {
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
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Out1, [this, pulseDuration](uint16_t data) {
            _eventQueue->removeCallbackEvent(_out1OutputQueueId);

            writeOut1Signal(true);

            _out1OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                writeOut1Signal(false);
            }, pulseDuration);
        });
        checkSendMapEntry(mapping, data, OutputMappingOutput_Out2, [this](uint16_t data) {
            _eventQueue->removeCallbackEvent(_out2OutputQueueId);

            writeOut2Signal(true);

            _out2OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                writeOut2Signal(false);
            }, _systemState->triggerDuration);
        });

        if (_systemState->expansionSensed) {
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX1, [this, pulseDuration](uint16_t data) {
                _eventQueue->removeCallbackEvent(_outX1OutputQueueId);

                writeOutX1Signal(true);

                _outX1OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                    writeOutX1Signal(false);
                }, pulseDuration);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX2, [this, pulseDuration](uint16_t data) {
                _eventQueue->removeCallbackEvent(_outX2OutputQueueId);

                writeOutX2Signal(true);

                _outX2OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                    writeOutX2Signal(false);
                }, pulseDuration);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX3, [this, pulseDuration](uint16_t data) {
                _eventQueue->removeCallbackEvent(_outX3OutputQueueId);

                writeOutX3Signal(true);

                _outX3OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                    writeOutX3Signal(false);
                }, pulseDuration);
            });
            checkSendMapEntry(mapping, data, OutputMappingOutput_OutX4, [this, pulseDuration](uint16_t data) {
                _eventQueue->removeCallbackEvent(_outX4OutputQueueId);

                writeOutX4Signal(true);

                _outX4OutputQueueId = _eventQueue->scheduleCallbackEvent([this] {
                    writeOutX4Signal(false);
                }, pulseDuration);
            });
        }
    });
}

void OutputController::addToCurrentNoteQueue(uint8_t note, uint8_t velocity) {
    sem_acquire_blocking(&_noteQueueSemaphore);

    NoteOnMapping *existing = nullptr;
    NoteOnMapping *current = _currentNotes;
    while (current != nullptr) {
        if (current->note == note) {
            existing = current;
            break;
        }
        current = current->next;
    }

    // if the note exists, update velocity and move it to the back of the queue
    if (existing != nullptr) {
        existing->velocity = velocity;

        // if we're already last in the queue - nothing to do
        if (_currentNotesQueueLast != existing) {
            // else, move this to the last
            if (existing->previous != nullptr) {
                existing->previous->next = existing->next;
            }
            if (existing->next != nullptr) {
                existing->next->previous = existing->previous;
            }
            if (_currentNotes == existing) {
                _currentNotes = existing->next;
            }
            if (_currentNotesQueueLast == nullptr) {
                _currentNotesQueueLast = existing;
            }  else {
                existing = nullptr;
                _currentNotesQueueLast->next = existing;
                existing->previous = _currentNotesQueueLast;
                _currentNotesQueueLast = existing;
            }
        }
    } else {
        // we're not in the queue, add to the end
        auto newNode = new NoteOnMapping {
            note, velocity, nullptr, nullptr
        };
        if (_currentNotesQueueLast == nullptr) {
            _currentNotes = newNode;
            _currentNotesQueueLast = newNode;
        } else {
            _currentNotesQueueLast->next = newNode;
            newNode->previous = _currentNotesQueueLast;
            _currentNotesQueueLast = newNode;
        }
    }

    sem_release(&_noteQueueSemaphore);
}

NoteOnMapping * OutputController::removeFromCurrentNoteQueue(uint8_t note) {
    // return the next note (head) in the queue
    NoteOnMapping *nextNote = nullptr;

    // turn off interrupts and lock
    sem_acquire_blocking(&_noteQueueSemaphore);

    NoteOnMapping *existing = nullptr;
    NoteOnMapping *current = _currentNotes;
    while (current != nullptr) {
        if (current->note == note) {
            existing = current;
            break;
        }
        current = current->next;
    }

    if (existing != nullptr) {
        if (existing->previous != nullptr) {
            existing->previous->next = existing->next;
        }
        if (existing->next != nullptr) {
            existing->next->previous = existing->previous;
        }
        if (_currentNotes == existing) {
            _currentNotes = existing->next;
        }
        if (_currentNotesQueueLast == existing) {
            _currentNotesQueueLast = existing->previous;
        }
        delete existing;
    }

    if (_currentNotes == nullptr)
        _currentNotesQueueLast = nullptr;

    nextNote = _currentNotes;

    sem_release(&_noteQueueSemaphore);

    return nextNote;
}

void OutputController::clearNoteQueue() {
    sem_acquire_blocking(&_noteQueueSemaphore);

    NoteOnMapping *current = _currentNotes;
    while (current != nullptr) {
        NoteOnMapping *next = current->next;

        delete current;
        current = next;
    }
    _currentNotes = nullptr;
    _currentNotesQueueLast = nullptr;

    sem_release(&_noteQueueSemaphore);
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
    } else if (noteValue >= DAC_7554_MAX_RANGE) {
        finalNoteValue = DAC_7554_MAX_RANGE - 1;
    }

    uint16_t rawFinalValue = finalNoteValue;
    if (_systemState->noteCVMaxVoltage == FiveVoltOutput) {
        // half the value for +5v output
        finalNoteValue = finalNoteValue >> 1;
    }

    _noteVelOut1Out2Output->writeNote(finalNoteValue);
    _currentState.currentNote = midiNote;
    sendCoreSignal(SignalCommand_NoteChange, midiNote);

    routeCVEvent(OutputMappingRoute_Note, rawFinalValue);
}

void OutputController::noteOnCallback(uint8_t midiNoteNumber, uint8_t velocity) {
    if (_ignoreMidi)
        return;

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
    if (_eventQueue->removeCallbackEvent(_lastTriggerQueueId)) {
        gpio_put(PIN_TRIGGER_LINE, false);
    }

    sendNoteWithBendAndAdjust(midiNoteNumber);

    uint16_t velocityValue = ten_volt_linear_12_bit_output[velocity];
    if (_systemState->velocityCVMaxVoltage == FiveVoltOutput) {
        velocityValue = velocityValue >> 1;
    }
    _noteVelOut1Out2Output->writeVelocity(velocityValue);
    _currentState.currentVelocity = velocity;
    sendCoreSignal(SignalCommand_VelocityChange, velocity);

    gpio_put(PIN_NOTE_LED, true);
    gpio_put(PIN_GATE_LINE, true);
    _currentState.gateState = true;

    gpio_put(PIN_TRIGGER_LINE, true);
    _currentState.triggerState = true;

    // and add our timed callback to turn off the trigger line
    _lastTriggerQueueId = _eventQueue->scheduleCallbackEvent([this] {
        gpio_put(PIN_TRIGGER_LINE, false);
        sendCoreSignal(SignalCommand_TriggerPulse_Off, 0);
        _currentState.triggerState = false;
    }, _systemState->triggerDuration);

    _lastNote = midiNoteNumber;

    addToCurrentNoteQueue(midiNoteNumber, velocity);

    sendCoreSignal(SignalCommand_TriggerPulse_On, 0);
    sendCoreSignal(SignalCommand_Gate_On, 0);

    routeCVEvent(OutputMappingRoute_Velocity, ten_volt_linear_12_bit_output[velocity]);
    routeSignalEvent(OutputMappingRoute_Gate, true);
    routePulseEvent(OutputMappingRoute_Trigger, _systemState->triggerDuration);
}

void OutputController::noteOffCallback(uint8_t note, uint8_t _) {
    NoteOnMapping *nextNote = removeFromCurrentNoteQueue(note);

    if (_ignoreMidi)
        return;

    if (nextNote != nullptr) {
        // re-trigger this note
        noteOnCallback(nextNote->note, nextNote->velocity);
        return;
    }

    if (_sustainValue && _lastNote != DEFAULT_LAST_NOTE_VALUE) {
        return;
    }

    if (_currentNotes == nullptr)
        allNotesOffCallback();
}

void OutputController::allNotesOffCallback() {
    if (_ignoreMidi)
        return;

    _eventQueue->removeCallbackEvent(_lastTriggerQueueId);
    _noteVelOut1Out2Output->writeNote(0);
    _noteVelOut1Out2Output->writeVelocity(0);

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

    routeCVEvent(OutputMappingRoute_Note, 0);
    routeCVEvent(OutputMappingRoute_Velocity, 0);
    routeSignalEvent(OutputMappingRoute_Gate, false);
    routePulseEvent(OutputMappingRoute_Trigger, false);
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
    if (_ignoreMidi)
        return;

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
    if (_ignoreMidi)
        return;

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
    if (_ignoreMidi)
        return;

    if (_lastNote != DEFAULT_LAST_NOTE_VALUE) {
        uint16_t velocityValue = ten_volt_linear_12_bit_output[velocity];
        if (_systemState->velocityCVMaxVoltage == FiveVoltOutput) {
            velocityValue = velocityValue >> 1;
        }

        _noteVelOut1Out2Output->writeVelocity(velocityValue);
        sendCoreSignal(SignalCommand_VelocityChange, velocity);

        _currentState.currentVelocity = velocity;

        routeCVEvent(OutputMappingRoute_Velocity, ten_volt_linear_12_bit_output[velocity]);
    }
}

void OutputController::onAftertouchCallback(uint8_t data) {
    if (_ignoreMidi)
        return;

    routeCVEvent(OutputMappingRoute_Aftertouch, data);
}

void OutputController::onExpressionCallback(uint8_t data) {
    if (_ignoreMidi)
        return;

    routeCVEvent(OutputMappingRoute_Expression, data);
}

void OutputController::onModWheelCallback(uint8_t data) {
    if (_ignoreMidi)
        return;

    routeCVEvent(OutputMappingRoute_ModWheel, data);
}

void OutputController::onEffectOneCallback(uint8_t data) {
    if (_ignoreMidi)
        return;

    routeCVEvent(OutputMappingRoute_Effect_1, data);
}

void OutputController::onEffectTwoCallback(uint8_t data) {
    if (_ignoreMidi)
        return;

    routeCVEvent(OutputMappingRoute_Effect_2, data);
}

void OutputController::onResetCallback() {
    if (_ignoreMidi)
        return;

    // stop and clear any incoming messages
    global_midi_controller->stop();

    // remove any timed event items
    _eventQueue->removeCallbackEvent(_clockCallbackQueueId);
    _eventQueue->removeCallbackEvent(_lastTriggerQueueId);

    // turn off any note
    noteOffCallback(DEFAULT_LAST_NOTE_VALUE, 0);

    writeOut1Signal(false);
    writeOut2Signal(false);

    if (_systemState->expansionSensed) {
        writeOutX1Signal(false);
        writeOutX2Signal(false);
        writeOutX3Signal(false);
        writeOutX4Signal(false);
    }

    gpio_put(PIN_CLOCK_LED, false);
    gpio_put(PIN_CLOCK_LINE, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);
    sendCoreSignal(SignalCommand_Reset, 0);

    routePulseEvent(OutputMappingRoute_Reset, RESET_PULSE_DURATION_MS);

    _clockTickCount = 0;
    _currentState = RunningState();

    // restart processing midi messages
    global_midi_controller->start();
}

void OutputController::onClockCallback() {
    if (_ignoreMidi)
        return;

    _clockTickCount++;

    routePulseEvent(OutputMappingRoute_ClockTick, CLOCK_PULSE_MS);

    if ((_clockTickCount % 2) == 0) {
        routePulseEvent(OutputMappingRoute_ClockTick_2, CLOCK_PULSE_MS);
    }

    if ((_clockTickCount % 4) == 0) {
        routePulseEvent(OutputMappingRoute_ClockTick_4, CLOCK_PULSE_MS);
    }

    if ((_clockTickCount % 6) == 0) {
        routePulseEvent(OutputMappingRoute_ClockTick_6, CLOCK_PULSE_MS);
    }

    if ((_clockTickCount % 8) == 0) {
        routePulseEvent(OutputMappingRoute_ClockTick_8, CLOCK_PULSE_MS);
    }

    if ((_clockTickCount % 12) == 0) {
        routePulseEvent(OutputMappingRoute_ClockTick_12, CLOCK_PULSE_MS);
    }

    if (_clockTickCount == 24) {
        routePulseEvent(OutputMappingRoute_ClockTick_24, CLOCK_PULSE_MS);
    }

    if (_clockTickCount >= 24) {
        _clockTickCount = 0;
    }
}

void OutputController::onStartCallback() {
    if (_ignoreMidi)
        return;

    routeSignalEvent(OutputMappingRoute_Run, true);
}

void OutputController::onStopCallback() {
    if (_ignoreMidi)
        return;

    routeSignalEvent(OutputMappingRoute_Run, false);
    routePulseEvent(OutputMappingRoute_Reset, RESET_PULSE_DURATION_MS);
}
