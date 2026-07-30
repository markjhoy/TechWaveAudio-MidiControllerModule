
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OutputVoltageSelectMenu.h"

#include "../SettingsMenuSystem.h"


void OutputVoltageSelectMenu::init() {
    _currentSelection = 0;
    setupMenuItems();
}

void OutputVoltageSelectMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->showMenu("Output Voltages", _menuItems.data(), _currentSelection, (int)_menuItems.size());
}

void OutputVoltageSelectMenu::onEnterPressed() {
    switch (_currentSelection) {
        case 0: {
            _systemState->noteCVMaxVoltage = (_systemState->noteCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 1: {
            _systemState->velocityCVMaxVoltage = (_systemState->velocityCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 2: {
            _systemState->auxCVMaxVoltage = (_systemState->auxCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        case 3: {
            _systemState->ctlCVMaxVoltage = (_systemState->ctlCVMaxVoltage == TenVoltOutput)
                ? FiveVoltOutput : TenVoltOutput;
        } break;
        default: {}
    }
    setupMenuItems();
    display();
}

void OutputVoltageSelectMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void OutputVoltageSelectMenu::onUpPressed() {
    _currentSelection--;
    if (_currentSelection < 0) {
        _currentSelection = 3;
    }
    display();
}

void OutputVoltageSelectMenu::onDownPressed() {
    _currentSelection++;
    if (_currentSelection > 3) {
        _currentSelection = 0;
    }
    display();
}

void OutputVoltageSelectMenu::setupMenuItems() {
    _menuItems.clear();
    if (_systemState->noteCVMaxVoltage == TenVoltOutput) {
        _menuItems.push_back("Note: +10v");
    } else {
        _menuItems.push_back("Note: +5v");
    }
    if (_systemState->velocityCVMaxVoltage == TenVoltOutput) {
        _menuItems.push_back("Vel:  +10v");
    } else {
        _menuItems.push_back("Vel:  +5v");
    }
    if (_systemState->auxCVMaxVoltage == TenVoltOutput) {
        _menuItems.push_back("Aux:  +10v");
    } else {
        _menuItems.push_back("Aux:  +5v");
    }
    if (_systemState->ctlCVMaxVoltage == TenVoltOutput) {
        _menuItems.push_back("Ctl:  +10v");
    } else {
        _menuItems.push_back("Ctl:  +5v");
    }
}
