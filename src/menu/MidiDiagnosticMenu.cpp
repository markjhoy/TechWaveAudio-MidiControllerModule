
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MidiDiagnosticMenu.h"

#include <iomanip>
#include <sstream>

#include "../SettingsMenuSystem.h"
#include "../io/MidiController.h"


extern MidiController *global_midi_controller;

void MidiDiagnosticMenu::display() {
    updateDisplay(true);
}

void MidiDiagnosticMenu::menuInit() {
    _customDisplay = true;

    global_midi_controller->stop();

    _logCount = -1;
    for (int i=0; i < MAX_MIDI_READ_LOG_MESSAGES; i++) {
        _logMessages[i] = "";
    }

    global_midi_controller->setOnResetCallback([this] { onResetCallback(); });
    global_midi_controller->setOnClockCallback([this] { onClockCallback(); });
    global_midi_controller->setOnAllNotesOffCallback([this] { allNotesOffCallback(); });
    global_midi_controller->setOnStartCallback([this] { onStartCallback(); });
    global_midi_controller->setOnStopCallback([this] { onStopCallback(); });

    global_midi_controller->setOnModWheelCallback([this](uint8_t data) { onModWheelCallback(data); });
    global_midi_controller->setOnSustainCallback([this](uint8_t data) { onSustainCallback(data); });
    global_midi_controller->setOnVolumeChangedCallback([this](uint8_t data) { onVolumeCallback(data); });
    global_midi_controller->setOnAftertouchCallback([this](uint8_t data) { onAftertouchCallback(data); });

    global_midi_controller->setOnNoteOnCallback([this](uint8_t data, uint8_t velocity) { noteOnCallback(data, velocity); });
    global_midi_controller->setOnNoteOffCallback([this](uint8_t data, uint8_t velocity) { noteOffCallback(data, velocity); });
    global_midi_controller->setOnPitchBendCallback([this](uint8_t highData, uint8_t lowData) { onPitchBendCallback(highData, lowData); });

    // listen to on all channels
    global_midi_controller->setChannel(0);

    global_midi_controller->start();
    _isExiting = false;
    _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { checkForMidiEvents(); }, 0);
}

bool MidiDiagnosticMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    _isExiting = true;

    // reset the state's midi channel
    global_midi_controller->setChannel(_systemState->midiChannel);

    global_midi_controller->stop();
    gpio_put(PIN_CLOCK_LED, false);

    _menuSystem->changeMenu(_previousMenu);
    return false;
}

void MidiDiagnosticMenu::updateDisplay(bool refresh) const {
    if (_isExiting) {
        return;
    }

    if (refresh) {
        _lcdDisplay->clear(true);
        _lcdDisplay->setTitle(" midi read log");
    }

    for (int i = 0; i < MAX_MIDI_READ_LOG_MESSAGES; i++) {
        _lcdDisplay->writeLineAt(i + 2, "                ", false, OledFontType_8x8);
        _lcdDisplay->writeLineAt(i + 2, _logMessages[i], false, OledFontType_8x8);
    }

    _lcdDisplay->show();
}

void MidiDiagnosticMenu::noteOnCallback(int note, int velocity) {
    _noteValue = note;
    _velocity = velocity;
    _octave = (note - 12) / 12;
    int whichNote = (note-24) % 12;
    _noteDisplay = note_names_display[whichNote];

    std::stringstream message;
    message << "N: " << _noteDisplay << " vel: " << _velocity;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::noteOffCallback(int noteNumber, int _) {
    _noteValue = 0;
    _velocity = 0;
    std::stringstream message;
    message << "-- note off " << noteNumber;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::allNotesOffCallback() {
    _noteValue = 0;
    _velocity = 0;
    addLogMessage("-- all notes off");
}

void MidiDiagnosticMenu::onModWheelCallback(int data) {
    std::stringstream message;
    message << "mod wh: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onPitchBendCallback(int fineValue, int coarseValue) {
    auto fullValue = static_cast<float>(((coarseValue << 8) + fineValue) & 0x4FFF);
    _pitchBendValue = (fullValue - 8192) / 8192.0f;
    std::stringstream message;
    message << "PB: " << _pitchBendValue;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onSustainCallback(int data) {
    std::stringstream message;
    message << "sustain: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onVolumeCallback(int velocity) {
    std::stringstream message;
    message << "volume: " << velocity;
    _velocity = velocity;
}

void MidiDiagnosticMenu::onAftertouchCallback(int data) {
    std::stringstream message;
    message << "after: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onExpressionCallback(int data) {
    std::stringstream message;
    message << "expr: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onEffectOneCallback(int data) {
    std::stringstream message;
    message << "fx_1: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onEffectTwoCallback(int data) {
    std::stringstream message;
    message << "fx_2: " << data;
    addLogMessage(message.str());
}

void MidiDiagnosticMenu::onResetCallback() {
    addLogMessage("!! reset !!");
}

void MidiDiagnosticMenu::onStartCallback() {
    addLogMessage("<< start >>");
}

void MidiDiagnosticMenu::onStopCallback() {
    addLogMessage("<< stop >>");
}

void MidiDiagnosticMenu::onClockCallback() {
    // don't log clock messages, but flash the led
    _lastClock = !_lastClock;
    gpio_put(PIN_CLOCK_LED, _lastClock);
}

void MidiDiagnosticMenu::checkForMidiEvents() {
    while (!_isExiting && global_midi_controller->processMidiQueue()) {
        tight_loop_contents();
    }

    if (!_isExiting) {
        _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { checkForMidiEvents(); }, 0);
    }
}

void MidiDiagnosticMenu::addLogMessage(const std::string &message) {
    _logCount++;
    if (_logCount >= MAX_MIDI_READ_LOG_MESSAGES) {
        for (int i = 0; i < (MAX_MIDI_READ_LOG_MESSAGES - 1); i++) {
            _logMessages[i] = _logMessages[i+1];
        }
        _logCount = MAX_MIDI_READ_LOG_MESSAGES - 1;
    }
    _logMessages[_logCount] = message;
    updateDisplay(false);
}
