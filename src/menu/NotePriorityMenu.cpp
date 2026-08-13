
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "NotePriorityMenu.h"

#include "../SettingsMenuSystem.h"

void NotePriorityMenu::menuInit() {
    std::vector<std::string> choices;
    choices.push_back("Last Note");
    choices.push_back("Highest Note");
    choices.push_back("Lowest Note");
    setMenuItems(choices);
    setCurrentSelectedOption(_systemState->notePriority);
}

bool NotePriorityMenu::onMenuItemSelected(int menuItemIndex) {
    _systemState->notePriority = static_cast<NotePriorityType>(menuItemIndex);
    setCurrentSelectedOption(menuItemIndex);
    return true;
}
