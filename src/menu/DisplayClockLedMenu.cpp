//
// Created by Mark J. Hoy on 2026-05-24.
//

#include "DisplayClockLedMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

void DisplayClockLedMenu::init() {
    _selectedValue = _systemState->clockTickLedCycle;
    for (int i = 0; i < NUM_CLOCK_TICK_LED_VALUES; i++) {
        if (_selectedValue == clock_led_toggle_values[i]) {
            _currentValue = i;
            break;
        }
    }
    setChoices();
}

void DisplayClockLedMenu::display() {
    if (_choices.empty()) {
        setChoices();
    }
    _lcdDisplay->showMenu("Clock LED Tick", _choices.data(), _currentValue, static_cast<int>(_choices.size()));
}

void DisplayClockLedMenu::onEnterPressed() {
    _systemState->clockTickLedCycle = clock_led_toggle_values[_currentValue];
    _selectedValue = _systemState->clockTickLedCycle;
    setChoices();
    display();
}

void DisplayClockLedMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void DisplayClockLedMenu::onUpPressed() {
    _currentValue--;
    if (_currentValue < 0) {
        _currentValue = NUM_CLOCK_TICK_LED_VALUES - 1;
    }
    display();
}

void DisplayClockLedMenu::onDownPressed() {
    _currentValue++;
    if (_currentValue >= NUM_CLOCK_TICK_LED_VALUES) {
        _currentValue = 0;
    }
    display();
}

void DisplayClockLedMenu::setChoices() {
    _choices.clear();
    int currentTickValue = _systemState->clockTickLedCycle;
    for (int i = 0; i < NUM_CLOCK_TICK_LED_VALUES; i++) {
        uint8_t thisValue = clock_led_toggle_values[i];
        std::stringstream thisItem;
        if (thisValue == currentTickValue) {
            thisItem << "* ";
        } else {
            thisItem << "  ";
        }
        if (thisValue == 0) {
            thisItem << "(led off)";
        } else if (thisValue == 1) {
            thisItem << "every tick";
        } else {
            thisItem << "every " << (int)thisValue << " ticks";
        }
        _choices.push_back(thisItem.str());
    }
}

