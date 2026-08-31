
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "DisplaySettingsMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

void DisplaySettingsMenu::menuInit() {
    resetMenuChoices();
}

bool DisplaySettingsMenu::onMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            _systemState->displayDashboard = !_systemState->displayDashboard;
            resetMenuChoices();
            return true;
        }
        case 1: {
            _menuSystem->changeMenu(_refreshMenu);
        } break;
        case 2: {
            _brightnessMenu->setCurrentValue(_systemState->screenBrightness);
            _brightnessMenu->setOnValueEditedCallback([this](const float value) {
                _lcdDisplay->setBrightness(static_cast<uint8_t>(value));
            });
            _menuSystem->changeMenu(_brightnessMenu);
        } break;
        case 3: {
            _menuSystem->changeMenu(_clockLedMenu);
        } break;;
        default: ;
    }
    return false;
}

void DisplaySettingsMenu::resetMenuChoices() {
    std::vector<std::string> menuChoices;
    if (_systemState->displayDashboard)
        menuChoices.push_back("hide dashboard");
    else
        menuChoices.push_back("show dashboard");

    menuChoices.push_back("display refresh");
    menuChoices.push_back("oled brightness");
    menuChoices.push_back("clock led rate");

    setMenuItems(menuChoices);
}


