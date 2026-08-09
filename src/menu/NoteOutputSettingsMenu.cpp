/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-08.
//

#include "NoteOutputSettingsMenu.h"

#include "../SettingsMenuSystem.h"

void NoteOutputSettingsMenu::display() {
    _lcdDisplay->clear();

    std::vector<std::string> choices = {
        "<< back",
        "pitch adjustment",
        "note priority",
        "pitch bend range"
    };
    if (_systemState->noteCVMaxVoltage == TenVoltOutput) {
        choices.push_back("max output +10v");
    } else {
        choices.push_back("max output +5v");
    }

    _lcdDisplay->showMenu(" Note Settings", choices.data(), _currentMenuPosition, static_cast<int>(choices.size()));
}

bool NoteOutputSettingsMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            // back
            _menuSystem->changeMenu(_previousMenu);
            return false;
        }
        case 1: {
            _pitchAdjustMenu->setCurrentValue(_systemState->pitchAdjust);
            _menuSystem->changeMenu(_pitchAdjustMenu);
            return false;
        }
        case 2: {
            _menuSystem->changeMenu(_notePriorityMenu);
            return false;
        }
        case 3: {
            _pitchBendRangeMenu->setCurrentValue(_systemState->pitchBendRange);
            _menuSystem->changeMenu(_pitchBendRangeMenu);
            return false;
        }
        case 4: {
            if (_systemState->noteCVMaxVoltage == TenVoltOutput) {
                _systemState->noteCVMaxVoltage = FiveVoltOutput;
            } else {
                _systemState->noteCVMaxVoltage = TenVoltOutput;
            }
            display();
            return false;
        }
        default: {
            return false;
        }
    }
}
