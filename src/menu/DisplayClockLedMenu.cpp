
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "DisplayClockLedMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

void DisplayClockLedMenu::menuInit() {
    for (int i = 0; i < NUM_CLOCK_TICK_LED_VALUES; i++) {
        if (_systemState->clockTickLedCycle == clock_led_toggle_values[i]) {
            _clockRateIndex = i;
            break;
        }
    }

    std::vector<std::string> choices;
    for (int i = 0; i < NUM_CLOCK_TICK_LED_VALUES; i++) {
        uint8_t thisValue = clock_led_toggle_values[i];
        std::stringstream thisItem;
        if (thisValue == 0) {
            thisItem << "(led off)";
        } else if (thisValue == 1) {
            thisItem << "every tick";
        } else {
            thisItem << "every " << static_cast<int>(thisValue) << " ticks";
        }
        choices.push_back(thisItem.str());
    }

    setMenuItems(choices);
    setCurrentMenuPosition(_clockRateIndex);
    setCurrentSelectedOption(_clockRateIndex);
}

bool DisplayClockLedMenu::onMenuItemSelected(int menuItemIndex) {
    _systemState->clockTickLedCycle = clock_led_toggle_values[menuItemIndex];
    _clockRateIndex = menuItemIndex;
    setCurrentSelectedOption(_clockRateIndex);
    return true;
}
