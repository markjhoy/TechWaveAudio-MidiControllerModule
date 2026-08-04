
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OutputVoltageSelectMenu.h"

#include "../SettingsMenuSystem.h"

void OutputVoltageSelectMenu::menuInit() {
    setupMenuChoices();
}

bool OutputVoltageSelectMenu::onMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            _systemState->noteCVMaxVoltage = (_systemState->noteCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 1: {
            _systemState->velocityCVMaxVoltage = (_systemState->velocityCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 2: {
            _systemState->out1CVMaxVoltage = (_systemState->out1CVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 3: {
            _systemState->out2CVMaxVoltage = (_systemState->out2CVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        default: {}
    }
    setupMenuChoices();
    return true;
}


void OutputVoltageSelectMenu::setupMenuChoices() {
    _menuChoices.clear();
    if (_systemState->noteCVMaxVoltage == TenVoltOutput) {
        _menuChoices.push_back("Note: +10v");
    } else {
        _menuChoices.push_back("Note: +5v");
    }
    if (_systemState->velocityCVMaxVoltage == TenVoltOutput) {
        _menuChoices.push_back("Vel:  +10v");
    } else {
        _menuChoices.push_back("Vel:  +5v");
    }
    if (_systemState->out1CVMaxVoltage == TenVoltOutput) {
        _menuChoices.push_back("Out1: +10v");
    } else {
        _menuChoices.push_back("Out1: +5v");
    }
    if (_systemState->out2CVMaxVoltage == TenVoltOutput) {
        _menuChoices.push_back("Out2: +10v");
    } else {
        _menuChoices.push_back("Out2: +5v");
    }
    setMenuItems(_menuChoices);
}
