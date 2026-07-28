//
// Created by Mark J. Hoy on 2026-07-28.
//

#include "OutputCVMappingMenu.h"

#include "../SettingsMenuSystem.h"

void OutputCVMappingMenu::init() {
    _availableRoutesNames.clear();
    _availableRoutes = this->getAvailableRoutes();
    for (int i = 0; i < _availableRoutes.size(); i++) {
        _availableRoutesNames.push_back(output_menu_route_choices[_availableRoutes[i]]);
    }

    OutputMappingRoute currentRoute = this->getCurrentRouteMapping();

    _setRouteMenuIndex = 0;
    for (int i = 0; i < _availableRoutes.size(); i++) {
        if (currentRoute == _availableRoutes[i]) {
            _setRouteMenuIndex = i;
            break;
        }
    }

    _currentMenuIndex = _setRouteMenuIndex;
}

void OutputCVMappingMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->showMenu(this->getMenuName(), _availableRoutesNames.data(), _currentMenuIndex,
                          static_cast<int>(_availableRoutesNames.size()), _setRouteMenuIndex);
    _lcdDisplay->show();
}

void OutputCVMappingMenu::onEnterPressed() {
    if (_currentMenuIndex == _setRouteMenuIndex)
        return;

    OutputMappingRoute newRoute = _availableRoutes[_currentMenuIndex];
    _setRouteMenuIndex = _currentMenuIndex;
    this->onRouteSettingChanged(newRoute);
    display();
}

void OutputCVMappingMenu::onBackPressed() {
    this->_menuSystem->changeMenu(_previousMenu);
}

void OutputCVMappingMenu::onUpPressed() {
    _currentMenuIndex--;
    if (_currentMenuIndex < 0)
        _currentMenuIndex = static_cast<int>(_availableRoutes.size()) - 1;
    display();
}

void OutputCVMappingMenu::onDownPressed() {
    _currentMenuIndex++;
    if (_currentMenuIndex >= static_cast<int>(_availableRoutes.size()))
        _currentMenuIndex = 0;
    display();
}
