
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MainMenu.h"

#include "../SettingsMenuSystem.h"
#include "TuningMenu.h"

static std::string main_menu_selections[] = {
    "Output Routings",
    "Input Settings",
    "Output Settings",
    "Display Settings",
    "Tuning",
    "About",
    "Reset All"
};

MainMenu::MainMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState)
: BaseMenu(lcdDisplay, menuSystem, systemState, nullptr)
{
    _outputRouteSettingsMenu = new OutputRouteSettingsMenu(_lcdDisplay, _menuSystem, systemState, this);
    _inputSettingsMenu = new InputSettingsMenu(_lcdDisplay, _menuSystem, systemState, this);
    _outputSettingsMenu = new OutputSettingsMenu(_lcdDisplay, _menuSystem, systemState, this);
    _displaySettingsMenu = new DisplaySettingsMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _tuningMenu = new TuningMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _aboutMenu = new AboutMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _resetMenu = new ResetMenu(_lcdDisplay,_menuSystem,_systemState,this);
}

MainMenu::~MainMenu() {
    delete _outputRouteSettingsMenu;
    delete _inputSettingsMenu;
    delete _outputSettingsMenu;
    delete _displaySettingsMenu;
    delete _tuningMenu;
    delete _aboutMenu;
    delete _resetMenu;
}

void MainMenu::menuInit() {
    setMenuItems(main_menu_selections, MAIN_MENU_NUM_ITEMS);
    if (_lastMenuItem == MAIN_MENU_RESET_ALL)
        _lastMenuItem = MAIN_MENU_OUTPUT_ROUTING;
    setCurrentMenuPosition(_lastMenuItem);
}

bool MainMenu::onMenuItemSelected(int menuItemIndex) {
    _lastMenuItem = menuItemIndex;
    switch (menuItemIndex) {
        case MAIN_MENU_OUTPUT_ROUTING: {
            _menuSystem->changeMenu(_outputRouteSettingsMenu);
        } break;
        case MAIN_MENU_INPUT_SETTINGS: {
            _menuSystem->changeMenu(_inputSettingsMenu);
        } break;
        case MAIN_MENU_OUTPUT_SETTINGS:  {
            _menuSystem->changeMenu(_outputSettingsMenu);
        } break;
        case MAIN_MENU_DISPLAY_SETTINGS: {
            _menuSystem->changeMenu(_displaySettingsMenu);
        };break;
        case MAIN_MENU_UTILITIES: {
            _menuSystem->changeMenu(_tuningMenu);
        } break;
        case MAIN_MENU_ABOUT: {
            _menuSystem->changeMenu(_aboutMenu);
        } break;
        case MAIN_MENU_RESET_ALL: {
            _menuSystem->changeMenu(_resetMenu);
        } break;
        default: {
            return true;
        }
    }
    return false;
}

bool MainMenu::onBeforeLeftRotation(int currentMenuItemIndex) {
    if (currentMenuItemIndex == MAIN_MENU_RESET_ALL || currentMenuItemIndex == MAIN_MENU_ABOUT) {
        _lastMenuItem = 0;
    } else {
        _lastMenuItem = currentMenuItemIndex;
    }
    return true;
}

bool MainMenu::onBeforeRightRotation(int currentMenuItemIndex) {
    if (currentMenuItemIndex == MAIN_MENU_RESET_ALL || currentMenuItemIndex == MAIN_MENU_ABOUT) {
        _lastMenuItem = 0;
    } else {
        _lastMenuItem = currentMenuItemIndex;
    }
    return true;
}

