//
// Created by Mark J. Hoy on 2026-05-02.
//

#include "DashboardDisplay.h"

#include <cstring>
#include <sstream>

DashboardDisplay::DashboardDisplay(OledDisplay *lcdDisplay, SystemState *systemState) {
    _lcdDisplay = lcdDisplay;
    _systemState = systemState;
    _displayLines = new char[OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES];
    setDefaultTemplate();
}

void DashboardDisplay::display() {
    setDefaultTemplate();
    _lcdDisplay->clear(false);
    _lcdDisplay->writeLines(_displayLines, OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES);

    update();
}

void DashboardDisplay::update() {
    if (!_systemState->displayDashboard) {
        if (!_isDashboardCleared) {
            _lcdDisplay->clear(true);
        }
        _isDashboardCleared = true;
        return;
    }

    ScreenXYPos pos;

    _isDashboardCleared = false;

    pos = dashboard_value_position[DASHBOARD_VALUE_CHANNEL];
    std::stringstream displayValue;
    if (_currentState.midiChannel == 0) {
        displayValue << "--";
    } else {
        if (_currentState.midiChannel < 10) {
            displayValue << _currentState.midiChannel << " ";
        } else {
            displayValue << _currentState.midiChannel;
        }
    }
    _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, displayValue.str(), true);

    pos = dashboard_value_position[DASHBOARD_VALUE_NOTE];
    if (_currentState.currentNote == DEFAULT_LAST_NOTE_VALUE) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "___", true);
    } else {
        int octave = (_currentState.currentNote - 12) / 12;
        int whichNote = _currentState.currentNote % 12;
        auto noteDisplayValue = note_names_display[whichNote];
        std::stringstream noteDisplay;
        noteDisplay << noteDisplayValue << octave;
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, noteDisplay.str(), true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_VELOCITY];
    if (_currentState.currentVelocity == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "___", true);
    } else {
        std::stringstream displayValue;
        displayValue << (int)_currentState.currentVelocity << " ";
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, displayValue.str(), true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_AUX];
    if (_currentState.currentAux == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "__", true);
    } else {
        std::stringstream displayValue;
        displayValue << (int)_currentState.currentAux << " ";
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, displayValue.str(), true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_CC];
    if (_currentState.currentCtl == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "__", true);
    } else {
        std::stringstream displayValue;
        displayValue << (int)_currentState.currentCtl << " ";
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, displayValue.str(), true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_TRIGGER];
    if (_currentState.triggerState) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "#", true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_", true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_GATE];
    if (_currentState.gateState) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "#", true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_", true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_CLOCK];
    if (_currentState.clockState) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "#", true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_", true);
    }

    _lastDot = !_lastDot;
    if (_lastDot) {
        _lcdDisplay->writeTextAt(0, OLED_NUM_TEXT_LINES - 1, ".", true);
    } else {
        _lcdDisplay->writeTextAt(0, OLED_NUM_TEXT_LINES - 1, " ", true);
    }

    _lcdDisplay->show();
}

void DashboardDisplay::setCurrentState(DashboardState_t *state) {
    _currentState = *state;
}

void DashboardDisplay::setDefaultTemplate() {
    char *buf = _displayLines;
    for (const auto & l : default_dashboard_template) {
        memcpy(buf, l.c_str(), OLED_NUM_CHARS_PER_LINE);
        buf += OLED_NUM_CHARS_PER_LINE;
    }
}
