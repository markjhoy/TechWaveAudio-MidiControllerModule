
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "TuningMenu.h"

#include <sstream>

#include "../SettingsMenuSystem.h"
#include "../GlobalHandlers.h"

static std::string tuning_menu_selections[] = {
    "A4 (440)", // note 69
    "C4 middle", // note 60
    "C0", // note 12, +10v only
    "C1", // note 24, +10v only
    "C2", // note 36
    "C3", // note 48
    "C5", // note 72
    "C6", // note 84
    "C7", // note 96, +10v only
    "C8", // note 108, +10v only
    "C9", // note 120, +10v only
};

static uint8_t tuning_menu_note_values[] = {
    69, 60, 12, 24, 36, 48, 72, 84, 96, 108, 120
};

void TuningMenu::display() {
    std::stringstream title;
    title << "Tuning " << (_systemState->noteCVMaxVoltage == FiveVoltOutput ? "+5v" : "+10v");
    _lcdDisplay->showMenu(title.str(), _choices.data(), _selectedChoice, static_cast<int>(_choices.size()));
}

void TuningMenu::menuInit() {
    _customDisplay = true;

    // shutdown our global output controller
    global_core0_handler->turnOffGlobalOutputController();

    // and create our own
    _outputController = new OutputController(_systemState, _menuSystem->getTimerQueue());

    _choices.clear();
    _choices.push_back("<< back");
    _choiceNoteValues.clear();
    for (int i=0; i < TUNING_MENU_NUM_SELECTIONS; i++) {
        if (_systemState->noteCVMaxVoltage == FiveVoltOutput && (
                i == TUNING_MENU_SELECTION_C0 ||
                i == TUNING_MENU_SELECTION_C1 ||
                i == TUNING_MENU_SELECTION_C7 ||
                i == TUNING_MENU_SELECTION_C8 ||
                i == TUNING_MENU_SELECTION_C9
            )
        ) {
            continue;
        }
        _choices.push_back(tuning_menu_selections[i]);
        _choiceNoteValues.push_back(tuning_menu_note_values[i]);
    }

    _selectedChoice = 0;
    _outputDac = _outputController->getNoteVelOut1Out2Output();
}

bool TuningMenu::onMenuItemSelected(int menuItemIndex) {
    // never called since custom display
    return false;
}

bool TuningMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    // enter was pressed
    if (_isTuning) {
        _isClosing = true;
        _isTuning = false;
        return false;
    }
    if (_isClosing) {
        return false;
    }

    if (_selectedChoice == 0) {
        // back was pressed
        // delete our own OutputController
        _outputController->shutdown();
        delete _outputController;

        // restart the global output controller
        global_core0_handler->turnOnGlobalOutputController();
        _menuSystem->changeMenu(_previousMenu);
        return false;
    }

    _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] {
        // subtract 1 from the values as we have a << back at 0
        this->performTuning(_choices[_selectedChoice], _choiceNoteValues[_selectedChoice - 1]);
    }, 0);
    return false;
}

bool TuningMenu::onBeforeLeftRotation(int currentMenuItemIndex) {
    if (_isTuning || _isClosing) {
        return false;
    }

    _selectedChoice--;
    if (_selectedChoice < 0) {
        _selectedChoice = static_cast<int>(_choices.size()) - 1;
    }

    display();
    return false;
}

bool TuningMenu::onBeforeRightRotation(int currentMenuItemIndex) {
    if (_isTuning || _isClosing) {
        return false;
    }

    _selectedChoice++;
    if (_selectedChoice >= static_cast<int>(_choices.size())) {
        _selectedChoice = 0;
    }

    display();
    return false;
}

void TuningMenu::reset() {
    _outputDac->writeNote(0);

    gpio_put(PIN_NOTE_LED, false);
    gpio_put(PIN_GATE_LINE, false);

    display();

    _isTuning = false;
    _isClosing = false;
}

void TuningMenu::performTuning(const std::string& noteName, const uint8_t noteValue) {
    _isTuning = true;
    _isClosing = false;

    _lcdDisplay->clear();
    _lcdDisplay->setTitle("  [[ Tuning ]]");
    std::stringstream noteDisplay;
    noteDisplay << "Note: " << noteName;
    _lcdDisplay->writeLineAt(2, noteDisplay.str(), false);
    _lcdDisplay->show();

    gpio_put(PIN_NOTE_LED, true);
    gpio_put(PIN_GATE_LINE, true);

    if (_systemState->noteCVMaxVoltage == FiveVoltOutput) {
        auto outputValue = five_volt_note_12_bit_output[noteValue - MIDI_MIN_NOTE_5V];
        _outputDac->writeNote(outputValue);
    } else {
        auto outputValue = ten_volt_note_12_bit_output[noteValue - 12];
        _outputDac->writeNote(outputValue);
    }

    while (_isTuning) {
        tight_loop_contents();
    }

    reset();
}
