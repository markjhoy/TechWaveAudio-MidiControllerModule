//
// Created by Mark J. Hoy on 2026-05-25.
//

#include "ControlOutputMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

static std::string menu_control_output_choices[] = {
    "Mod Wheel",
    "Effect 1",
    "Effect 2"
};

void ControlOutputMenu::init() {
    _currentSelection = CTL_SETTING_MOD_WHEEL;
    _currentActiveSelection = _systemState->ctlMode;
}

void ControlOutputMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->showMenu("Control Output", menu_control_output_choices, _currentSelection, 3, _currentActiveSelection);
    _lcdDisplay->show();
}

void ControlOutputMenu::onEnterPressed() {
    _systemState->ctlMode = _currentSelection;
    display();
}

void ControlOutputMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void ControlOutputMenu::onUpPressed() {
    switch (_currentSelection) {
        case CTL_SETTING_MOD_WHEEL: break;
        case CTL_SETTING_EFFECT_1: {
            _currentSelection = CTL_SETTING_MOD_WHEEL;
        } break;
        case CTL_SETTING_EFFECT_2: {
            _currentSelection = CTL_SETTING_EFFECT_1;
        } break;
    }
    display();
}

void ControlOutputMenu::onDownPressed() {
    switch (_currentSelection) {
        case CTL_SETTING_MOD_WHEEL: {
            _currentSelection = CTL_SETTING_EFFECT_1;
        } break;
        case CTL_SETTING_EFFECT_1: {
            _currentSelection = CTL_SETTING_EFFECT_2;
        } break;
        case CTL_SETTING_EFFECT_2: break;
    }
    display();
}
