/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "BaseMenu.h"

#include "../SettingsMenuSystem.h"

void BaseMenu::init() {
    // always add the back option to the top
    _menuItems.clear();
    menuInit();

    // set menu position to 1 so we're not on << back
    _currentMenuPosition = _lastSelectedMenuItemIndex == 0 ? 1 : _lastSelectedMenuItemIndex;

    display();
}

void BaseMenu::display() {
    if (_customDisplay)
        return;

    _lcdDisplay->showMenu(
        getMenuName(),
        _menuItems.data(),
        _currentMenuPosition,
        static_cast<int>(_menuItems.size()),
        _currentSelectedOption
    );
}

void BaseMenu::onEnterPressed() {
    if (!onBeforeMenuItemSelected(_currentMenuPosition))
        return;

    if (_customDisplay)
        return;

    if (_currentMenuPosition == 0) {
        // back pressed
        if (onBackPressed()) {
            _menuSystem->changeMenu(_previousMenu);
            return;
        }
    }

    if (onMenuItemSelected(_currentMenuPosition - 1)) {
        display();
    } else {
        _lastSelectedMenuItemIndex = _currentMenuPosition;
    }
}

void BaseMenu::onLeftRotation() {
    if (!onBeforeLeftRotation(_currentMenuPosition - 1))
        return;

    if (_customDisplay)
        return;

    _currentMenuPosition--;
    if (_currentMenuPosition < 0) {
        _currentMenuPosition = static_cast<int>(_menuItems.size()) - 1;
    }
    display();
}

void BaseMenu::onRightRotation() {
    if (!onBeforeRightRotation(_currentMenuPosition - 1))
        return;

    if (_customDisplay)
        return;

    _currentMenuPosition++;
    if (_currentMenuPosition >= static_cast<int>(_menuItems.size())) {
        _currentMenuPosition = 0;
    }
    display();
}

void BaseMenu::setMenuItems(const std::vector<std::string> &menuItems) {
    setMenuItems(menuItems.data(), static_cast<int>(menuItems.size()));
}

void BaseMenu::setMenuItems(const std::string *menuItems, int numItems) {
    _menuItems.clear();
    _menuItems.push_back("<< back");
    for (int i = 0; i < numItems; ++i) {
        _menuItems.push_back(menuItems[i]);
    }
}

void BaseMenu::changeMenuItem(int index, const std::string &newItem) {
    if (index < 0 || index >= (_menuItems.size() - 1)) {
        return;
    }

    // add 1 to account for << back
    _menuItems[index + 1] = newItem;
}

void BaseMenu::setCurrentMenuPosition(int index) {
    if (index < 0) {
        return;
    }
    if (index == 0 || index >= (_menuItems.size() - 1))
        _currentMenuPosition = 1;
    else
        _currentMenuPosition = index + 1;
}

void BaseMenu::setCurrentSelectedOption(int index) {
    if (index < 0 || index >= (_menuItems.size() - 1)) {
        return;
    }

    // add 1 to account for << back
    _currentSelectedOption = index + 1;
}
