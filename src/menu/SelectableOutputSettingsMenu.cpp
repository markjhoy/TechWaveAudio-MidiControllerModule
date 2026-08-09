/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-05.
//

#include "SelectableOutputSettingsMenu.h"

void SelectableOutputSettingsMenu::display() {
    _lcdDisplay->clear();

    std::stringstream routingStr;
    routingStr << _outputName << " routing";

    std::vector<std::string> menuItems = {
        "<< back",
        routingStr.str(),
    };

    if (_getCurrentVoltageOutput() == TenVoltOutput) {
        menuItems.push_back("max output +10v");
    } else {
        menuItems.push_back("max output +5v");
    }

    _lcdDisplay->showMenu(_menuTitle, menuItems.data(), _currentMenuPosition, static_cast<int>(menuItems.size()));
}

bool SelectableOutputSettingsMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    switch (menuItemIndex) {
        case 0: {
            // back
            _menuSystem->changeMenu(_previousMenu);
            return false;
        }
        case 1: {
            // routing
            _menuSystem->changeMenu(_routeMappingMenu);
            return false;
        }
        case 2: {
            if (_getCurrentVoltageOutput() == TenVoltOutput) {
                _onOutputVoltageChanged(FiveVoltOutput);
            } else {
                _onOutputVoltageChanged(TenVoltOutput);
            }
            display();
            return false;
        }
        default: {
            return false;
        }
    }
}

