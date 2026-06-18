//
// Created by Mark J. Hoy on 2026-05-07.
//

#include "MidiDiagnosticMenu.h"

#include <iomanip>
#include <sstream>

#include "../SettingsMenuSystem.h"

extern MidiController *global_midi_controller;

void MidiDiagnosticMenu::init() {
    global_midi_controller->stop();

    _logCount = -1;
    for (int i=0; i < 3; i++) {
        _logMessages[i] = "";
    }

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

    global_midi_controller->start();
    _isExiting = false;
}

void MidiDiagnosticMenu::display() {
    updateDisplay(true);
}

void MidiDiagnosticMenu::onEnterPressed() {
    // nothing
}

void MidiDiagnosticMenu::onBackPressed() {
    _isExiting = true;

    global_midi_controller->stop();
    gpio_put(PIN_CLOCK_LED, false);

    _menuSystem->changeMenu(_previousMenu);
}

void MidiDiagnosticMenu::onUpPressed() {
    // nothing
}

void MidiDiagnosticMenu::onDownPressed() {
    // nothing
}

void MidiDiagnosticMenu::updateDisplay(bool refresh) {
    if (_isExiting) {
        return;
    }

    if (refresh) {
        _lcdDisplay->clear(true);
        _lcdDisplay->setTitle("   midi test");
    }

    for (int i = 0; i < 3; i++) {
        _lcdDisplay->writeLineAt(i + 1, "                ");
        _lcdDisplay->writeLineAt(i + 1, _logMessages[i]);
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

void MidiDiagnosticMenu::onClockCallback() {
    // don't log clock messages, but flash the led
    _lastClock = !_lastClock;
    gpio_put(PIN_CLOCK_LED, _lastClock);
}

void MidiDiagnosticMenu::addLogMessage(const std::string &message) {
    _logCount++;
    if (_logCount >= 3) {
        _logMessages[0] = _logMessages[1];
        _logMessages[1] = _logMessages[2];
        _logCount = 2;
    }
    _logMessages[_logCount] = message;
    updateDisplay(false);
}
