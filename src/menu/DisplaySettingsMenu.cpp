
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "DisplaySettingsMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

void DisplaySettingsMenu::init() {
    _currentValue = 0;
}

void DisplaySettingsMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->setTitle("Display Settings");

    if (_systemState->displayDashboard) {
        _lcdDisplay->writeLineAt(1, "Hide dashboard", _currentValue == 0);
    } else {
        _lcdDisplay->writeLineAt(1, "Show dashboard", _currentValue == 0);
    }
    _lcdDisplay->writeLineAt(2, "Display refresh", _currentValue == 1);
    _lcdDisplay->writeLineAt(3, "Clk led refresh", _currentValue == 2);

    _lcdDisplay->show();
}

void DisplaySettingsMenu::onEnterPressed() {
    if (_currentValue == 0) {
        _systemState->displayDashboard = !_systemState->displayDashboard;
        display();
    } else if (_currentValue == 1) {
        _menuSystem->changeMenu(_refreshMenu);
    } else if (_currentValue == 2) {
        _menuSystem->changeMenu(_clockLedMenu);
    }
}

void DisplaySettingsMenu::onBackPressed() {
    _menuSystem->changeMenu(this->_previousMenu);
}

void DisplaySettingsMenu::onNextPressed() {
    // do nothing
}

void DisplaySettingsMenu::onUpPressed() {
    _currentValue--;
    if (_currentValue < 0) {
        _currentValue = 2;
    }
    display();
}

void DisplaySettingsMenu::onDownPressed() {
    _currentValue++;
    if (_currentValue > 2) {
        _currentValue = 0;
    }
    display();
}
