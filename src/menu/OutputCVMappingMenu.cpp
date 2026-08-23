
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OutputCVMappingMenu.h"

#include "../SettingsMenuSystem.h"

void OutputCVMappingMenu::menuInit() {
    _availableRoutes = this->getAvailableRoutes();

    std::vector<std::string> availableRoutes;
    availableRoutes.reserve(_availableRoutes.size());
    for (int i = 0; i < _availableRoutes.size(); i++) {
        availableRoutes.push_back(output_menu_route_choices[_availableRoutes[i]]);
    }
    setMenuItems(availableRoutes);

    OutputMappingRoute currentRoute = this->getCurrentRouteMapping();

    _currentRoutingIndex = 0;
    for (int i = 0; i < _availableRoutes.size(); i++) {
        if (currentRoute == _availableRoutes[i]) {
            _currentRoutingIndex = i;
            break;
        }
    }

    setCurrentSelectedOption(_currentRoutingIndex);
    setCurrentMenuPosition(_currentRoutingIndex);
}

bool OutputCVMappingMenu::onMenuItemSelected(int menuItemIndex) {
    if (menuItemIndex == _currentRoutingIndex)
        return false;

    OutputMappingRoute newRoute = _availableRoutes[menuItemIndex];
    _currentRoutingIndex = menuItemIndex;
    this->onRouteSettingChanged(newRoute);

    setCurrentSelectedOption(menuItemIndex);

    return true;
}
