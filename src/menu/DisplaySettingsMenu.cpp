
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
    std::vector<std::string> menuChoices;
    if (_systemState->displayDashboard)
        menuChoices.push_back("hide dashboard");
    else
        menuChoices.push_back("hide dashboard");

    menuChoices.push_back("display refresh");
    menuChoices.push_back("clock led rate");

    setMenuItems(menuChoices);
}

bool DisplaySettingsMenu::onMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            _systemState->displayDashboard = !_systemState->displayDashboard;
            return true;
        }
        case 1: {
            _menuSystem->changeMenu(_refreshMenu);
        } break;
        case 2: {
            _menuSystem->changeMenu(_clockLedMenu);
        } break;;
    }
    return false;
}


