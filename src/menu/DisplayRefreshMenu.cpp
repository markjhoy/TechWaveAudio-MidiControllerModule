
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "DisplayRefreshMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

void DisplayRefreshMenu::init() {
    _selectedValue = _systemState->dashboardRefreshMs;
    for (int i = 0; i < NUM_DASHBOARD_UPDATE_VALUES; i++) {
        if (_selectedValue == display_update_values[i]) {
            _currentValue = i;
            break;
        }
    }
    setChoices();
}

void DisplayRefreshMenu::display() {
    if (_choices.empty()) {
        setChoices();
    }
    _lcdDisplay->showMenu("Display Refresh", _choices.data(), _currentValue, static_cast<int>(_choices.size()));
}

void DisplayRefreshMenu::onEnterPressed() {
    _systemState->dashboardRefreshMs = display_update_values[_currentValue];
    _selectedValue = _systemState->dashboardRefreshMs;
    setChoices();
    display();
}

void DisplayRefreshMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void DisplayRefreshMenu::onUpPressed() {
    _currentValue--;
    if (_currentValue < 0) {
        _currentValue = NUM_DASHBOARD_UPDATE_VALUES - 1;
    }
    display();
}

void DisplayRefreshMenu::onDownPressed() {
    _currentValue++;
    if (_currentValue >= NUM_DASHBOARD_UPDATE_VALUES) {
        _currentValue = 0;
    }
    display();
}

void DisplayRefreshMenu::setChoices() {
    _choices.clear();
    uint32_t currentDuration = _systemState->dashboardRefreshMs;
    for (int i = 0; i < NUM_DASHBOARD_UPDATE_VALUES; i++) {
        uint32_t thisValue = display_update_values[i];
        std::stringstream thisItem;
        if (thisValue == currentDuration) {
            thisItem << "* " << thisValue << "ms";
        } else {
            thisItem << "  " << thisValue << "ms";
        }
        _choices.push_back(thisItem.str());
    }
}
