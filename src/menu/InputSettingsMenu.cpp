/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "InputSettingsMenu.h"

#include "../SettingsMenuSystem.h"

void InputSettingsMenu::menuInit() {
    std::vector<std::string> menuOptions = {
        "Midi Channel",
        "Note Priority",
    };
    setMenuItems(menuOptions);
}

bool InputSettingsMenu::onMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            _menuSystem->changeMenu(_midiChannelMenu);
            return false;
        }
        case 1: {
            _menuSystem->changeMenu(_notePriorityMenu);
            return false;
        }
        default: {
            return true;
        }
    }
}
