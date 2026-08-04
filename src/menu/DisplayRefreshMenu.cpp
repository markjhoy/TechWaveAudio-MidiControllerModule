
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include <sstream>
#include "DisplayRefreshMenu.h"

void DisplayRefreshMenu::menuInit() {
    std::vector<std::string> choices;
    int currentDurationIndex = 0;
    for (int i = 0; i < NUM_DASHBOARD_UPDATE_VALUES; i++) {
        uint32_t thisValue = display_update_values[i];
        if (thisValue == _systemState->dashboardRefreshMs)
            currentDurationIndex = i;

        std::stringstream thisItem;
        thisItem << thisValue << "ms";
        choices.push_back(thisItem.str());
    }
    setCurrentSelectedOption(currentDurationIndex);
}

bool DisplayRefreshMenu::onMenuItemSelected(int menuItemIndex) {
    _systemState->dashboardRefreshMs = display_update_values[menuItemIndex];
    setCurrentSelectedOption(menuItemIndex);
    return true;
}
