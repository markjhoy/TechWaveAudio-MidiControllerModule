
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


void TriggerDurationMenu::init() {
    _currentMenuItem = getDurationIndex(_systemState->triggerDuration);
    setChoices();
}

void TriggerDurationMenu::display() {
    if (_choices.empty()) {
        setChoices();
    }
    _lcdDisplay->showMenu("Trigger Duration", _choices.data(), _currentMenuItem, (int)_choices.size());
}

void TriggerDurationMenu::onEnterPressed() {
    auto duration = trigger_duration_values[_currentMenuItem];
    _systemState->triggerDuration = duration;
    setChoices();
    display();
}

void TriggerDurationMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void TriggerDurationMenu::onUpPressed() {
    _currentMenuItem -= 1;
    if (_currentMenuItem < 0) {
        _currentMenuItem = 0;
    }
    display();
}

void TriggerDurationMenu::onDownPressed() {
    _currentMenuItem += 1;
    if (_currentMenuItem >= TOTAL_NUM_TRIGGER_DURATIONS) {
        _currentMenuItem = TOTAL_NUM_TRIGGER_DURATIONS - 1;
    }
    display();
}

void TriggerDurationMenu::setChoices() {
    _choices.clear();
    int currentDuration = _systemState->triggerDuration;
    for (int i = 0; i < TOTAL_NUM_TRIGGER_DURATIONS; i++) {
        int thisValue = trigger_duration_values[i];
        std::stringstream thisItem;
        if (thisValue == currentDuration) {
            thisItem << "* " << thisValue << "ms";
        } else {
            thisItem << "  " << thisValue << "ms";
        }
        _choices.push_back(thisItem.str());
    }
}

int TriggerDurationMenu::getDurationIndex(int duration) {
    for (int i = 0; i < TOTAL_NUM_TRIGGER_DURATIONS; i++) {
        if (duration == trigger_duration_values[i]) {
            return i;
        }
    }
    return 0;
}
