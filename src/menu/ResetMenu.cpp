
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "ResetMenu.h"

void ResetMenu::display() {
    _lcdDisplay->clear();
    if (_wasReset) {
        _lcdDisplay->setTitle(" Reset Complete");
        _lcdDisplay->writeLineAt(2, " Settings reset ", false);
        _lcdDisplay->writeLineAt(3, "   complete", false);
    } else {
        _lcdDisplay->setTitle("Reset Settings");
        _lcdDisplay->writeLineAt(1, "Confirm reset?", true);
        _lcdDisplay->writeLineAt(2, "   [ cancel ]", !_selectReset);
        _lcdDisplay->writeLineAt(3, "[confirm] reset", _selectReset);
    }
    _lcdDisplay->show();
}

void ResetMenu::menuInit() {
    _customDisplay = true;
    _wasReset = false;
    _selectReset = false;
    display();
}

bool ResetMenu::onMenuItemSelected(int menuItemIndex) {
    // never called
    return false;
}

bool ResetMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    if (_wasReset) {
        _menuSystem->changeMenu(_previousMenu);
        return false;
    }

    if (_selectReset) {
        // reset
        (static_cast<SettingsMenuSystem *>(_menuSystem))->resetState();
        if (_systemState->stateChanged) {
            (static_cast<SettingsMenuSystem *>(_menuSystem))->saveState();
        }
        _wasReset = true;
        display();
    } else {
        // cancel
        _menuSystem->changeMenu(_previousMenu);
    }
    return false;
}

bool ResetMenu::onBeforeLeftRotation(int currentMenuItemIndex) {
    if (_wasReset) {
        return false;
    }
    _selectReset = !_selectReset;
    display();
    return false;
}

bool ResetMenu::onBeforeRightRotation(int currentMenuItemIndex) {
    if (_wasReset) {
        return false;
    }
    _selectReset = !_selectReset;
    display();
    return false;
}
