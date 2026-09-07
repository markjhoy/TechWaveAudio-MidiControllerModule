
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "TriggerDurationMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

static int trigger_duration_values[] = {
    25, 50, 75, 100, 150, 200, 250, 300, 350, 400, 450, 500
};

void TriggerDurationMenu::menuInit() {
    std::vector<std::string> menuChoices;

    for (int i = 0; i < TOTAL_NUM_TRIGGER_DURATIONS; i++) {
        int thisValue = trigger_duration_values[i];
        std::stringstream thisItem;
        thisItem << thisValue << "ms";
        menuChoices.push_back(thisItem.str());
    }
    setMenuItems(menuChoices);

    int currentIndex = getDurationIndex(_systemState->triggerDuration);
    setCurrentMenuPosition(currentIndex);
    setCurrentSelectedOption(currentIndex);
}

bool TriggerDurationMenu::onMenuItemSelected(int menuItemIndex) {
    _systemState->triggerDuration = trigger_duration_values[menuItemIndex];
    setCurrentSelectedOption(menuItemIndex);
    return true;
}

int TriggerDurationMenu::getDurationIndex(int duration) {
    for (int i = 0; i < TOTAL_NUM_TRIGGER_DURATIONS; i++) {
        if (duration == trigger_duration_values[i]) {
            return i;
        }
    }
    return 0;
}
