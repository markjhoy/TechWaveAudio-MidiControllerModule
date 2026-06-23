//
// Created by Mark J. Hoy on 2026-06-01.
//

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

void TuningMenu::init() {
    // shutdown our global output controller
    global_core0_handler->turnOffGlobalOutputController();

    // and create our own
    _outputController = new OutputController(_systemState, _menuSystem->getTimerQueue());

    _choices.clear();
    _choiceNoteValues.clear();
    for (int i=0; i < TUNING_MENU_NUM_SELECTIONS; i++) {
        if (_systemState->noteCvOutput == FiveVoltOutput && (
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
    _noteOutput = _outputController->getNoteOutput();
}

void TuningMenu::display() {
    std::stringstream title;
    title << "Tuning " << (_systemState->noteCvOutput == FiveVoltOutput ? "+5v" : "+10v");
    _lcdDisplay->showMenu(title.str(), _choices.data(), _selectedChoice, static_cast<int>(_choices.size()));
}

void TuningMenu::onEnterPressed() {
    if (_isTuning || _isClosing) {
        return;
    }

    _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->performTuning(_choices[_selectedChoice], _choiceNoteValues[_selectedChoice]); }, 0);
}

void TuningMenu::onBackPressed() {
    if (_isTuning) {
        _isClosing = true;
        _isTuning = false;
        return;
    }
    if (_isClosing) {
        return;
    }

    // delete our own OutputController
    _outputController->shutdown();
    delete _outputController;

    // restart the global output controller
    global_core0_handler->turnOnGlobalOutputController();
    _menuSystem->changeMenu(_previousMenu);
}

void TuningMenu::onNextPressed() {
    // nothing
}

void TuningMenu::onUpPressed() {
    if (_isTuning || _isClosing) {
        return;
    }

    _selectedChoice--;
    if (_selectedChoice < 0) {
        _selectedChoice = static_cast<int>(_choices.size()) - 1;
    }

    display();
}

void TuningMenu::onDownPressed() {
    if (_isTuning || _isClosing) {
        return;
    }

    _selectedChoice++;
    if (_selectedChoice >= static_cast<int>(_choices.size())) {
        _selectedChoice = 0;
    }

    display();
}

void TuningMenu::reset() {
    _noteOutput->write(0);

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

    auto outputValue = ten_volt_note_12_bit_output[noteValue - 12];
    // TODO - proper note scale for 5v
    if (_systemState->noteCvOutput == FiveVoltOutput) {
        outputValue >>= 1;
    }
    _noteOutput->write(outputValue);

    while (_isTuning) {
        tight_loop_contents();
    }

    reset();
}
