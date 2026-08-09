/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-05.
//

#include "OutputSettingsMenu.h"

void OutputSettingsMenu::menuInit() {
    std::vector<std::string> choices = {
        "Note Output",
        "Velocity Output",
        "Out1 Output",
        "Out2 Output",
    };

    if (_systemState->expansionSensed) {
        choices.push_back("Out X1 Output");
        choices.push_back("Out X2 Output");
        choices.push_back("Out X3 Output");
        choices.push_back("Out X4 Output");
    }

    choices.push_back("Trig Pulse Width");
    choices.push_back("Clock Output");

    setMenuItems(choices);
}

bool OutputSettingsMenu::onMenuItemSelected(int menuItemIndex) {
    if (_systemState->expansionSensed)
        return onExpandedMenuItemSelected(menuItemIndex);

    switch (menuItemIndex) {
        case 0: {
            // note
            _menuSystem->changeMenu(_noteOutputSettingsMenu);
            return false;
        }
        case 1: {
            // velocity
            _menuSystem->changeMenu(_velocityOutputSettingsMenu);
            return false;
        }
        case 2: {
            // out 1
            _menuSystem->changeMenu(_out1SettingsMenu);
            return false;
        }
        case 3: {
            // out 2
            _menuSystem->changeMenu(_out2SettingsMenu);
            return false;
        }
        case 4: {
            // trg pulse
            _menuSystem->changeMenu(_triggerDurationMenu);
            return false;
        }
        case 5: {
            // clock
            _menuSystem->changeMenu(_clockOutputMenu);
            return false;
        }
        default: {
            return true;
        }
    }
}

bool OutputSettingsMenu::onExpandedMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            // note
            _menuSystem->changeMenu(_noteOutputSettingsMenu);
            return false;
        }
        case 1: {
            // velocity
            _menuSystem->changeMenu(_velocityOutputSettingsMenu);
            return false;
        }
        case 2: {
            // out 1
            _menuSystem->changeMenu(_out1SettingsMenu);
            return false;
        }
        case 3: {
            // out 2
            _menuSystem->changeMenu(_out2SettingsMenu);
            return false;
        }
        case 4: {
            // out x1
            _menuSystem->changeMenu(_outX1SettingsMenu);
            return false;
        }
        case 5: {
            // out x2
            _menuSystem->changeMenu(_outX2SettingsMenu);
            return false;
        }
        case 6: {
            // out x3
            _menuSystem->changeMenu(_outX3SettingsMenu);
            return false;
        }
        case 7: {
            // out x4
            _menuSystem->changeMenu(_outX4SettingsMenu);
            return false;
        }
        case 8: {
            // trg pulse
            _menuSystem->changeMenu(_triggerDurationMenu);
            return false;
        }
        case 9: {
            // clock
            _menuSystem->changeMenu(_clockOutputMenu);
            return false;
        }
        default: {
            return true;
        }
    }
}

