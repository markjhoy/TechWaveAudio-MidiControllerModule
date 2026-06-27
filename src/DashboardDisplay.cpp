//
// Created by Mark J. Hoy on 2026-05-02.
//

#include "DashboardDisplay.h"

#include <cstring>
#include <sstream>

DashboardDisplay::DashboardDisplay(OledDisplay *lcdDisplay, SystemState *systemState) {
    _lcdDisplay = lcdDisplay;
    _systemState = systemState;
    _textCharSize = _lcdDisplay->getTextCharacterSizing();
    _displayLines = new char[OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES];
    setDefaultTemplate();
}

void DashboardDisplay::display() {
    setDefaultTemplate();
    _lcdDisplay->clear(false);
    _lcdDisplay->writeLines(_displayLines, OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES, false);

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
            displayValue << (int)_currentState.midiChannel << " ";
        } else {
            displayValue << (int)_currentState.midiChannel;
        }
    }
    _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, displayValue.str());

    pos = dashboard_value_position[DASHBOARD_VALUE_NOTE];
    if (_currentState.currentNote == DEFAULT_LAST_NOTE_VALUE) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "___");
    } else {
        int octave = (_currentState.currentNote - 12) / 12;
        int whichNote = _currentState.currentNote % 12;
        auto noteDisplayValue = note_names_display[whichNote];
        std::stringstream noteDisplay;
        noteDisplay << noteDisplayValue << octave;
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, noteDisplay.str());
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_VELOCITY];
    if (_currentState.currentVelocity == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "___");
    } else {
        int rectWidth = (int)(_barWidthPerPart * (float)_currentState.currentVelocity);
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 3);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height;
        _lcdDisplay->drawRect(screenXPos, screenYPos, rectWidth, _textCharSize.height, true, true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_AUX];
    if (_currentState.currentAux == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "__");
    } else {
        int rectWidth = (int)(_barWidthPerPart * (float)_currentState.currentAux);
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 3);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height;
        _lcdDisplay->drawRect(screenXPos, screenYPos, rectWidth, _textCharSize.height, true, true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_CC];
    if (_currentState.currentCtl == 0) {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "__");
    } else {
        int rectWidth = (int)(_barWidthPerPart * (float)_currentState.currentCtl);
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 3);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height;
        _lcdDisplay->drawRect(screenXPos, screenYPos, rectWidth, _textCharSize.height, true, true);
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_TRIGGER];
    if (_currentState.triggerState) {
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 1);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height + 3;
        _lcdDisplay->drawRect(screenXPos, screenYPos, _textCharSize.width, _textCharSize.height-4, true, true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_");
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_GATE];
    if (_currentState.gateState) {
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 1);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height + 3;
        _lcdDisplay->drawRect(screenXPos, screenYPos, _textCharSize.width, _textCharSize.height-4, true, true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_");
    }

    pos = dashboard_value_position[DASHBOARD_VALUE_CLOCK];
    if (_currentState.clockState) {
        _lcdDisplay->clearTextArea(pos.xPos, pos.yPos, 1);
        int screenXPos = pos.xPos * _textCharSize.width;
        int screenYPos = pos.yPos * _textCharSize.height + 3;
        _lcdDisplay->drawRect(screenXPos, screenYPos, _textCharSize.width, _textCharSize.height-4, true, true);
    } else {
        _lcdDisplay->writeTextAt(pos.xPos, pos.yPos, "_");
    }

    _lastDot = !_lastDot;
    if (_lastDot) {
        _lcdDisplay->writeTextAt(0, OLED_NUM_TEXT_LINES - 1, ".");
    } else {
        _lcdDisplay->writeTextAt(0, OLED_NUM_TEXT_LINES - 1, " ");
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
