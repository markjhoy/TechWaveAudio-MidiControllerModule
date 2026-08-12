
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_ABOUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_ABOUTMENU_H
#include "BaseMenu.h"
#include "../SettingsMenuSystem.h"


class AboutMenu : public BaseMenu {
public:
    AboutMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    void display() override;

    std::string getMenuName() override { return "About"; }
protected:
    void menuInit() override {
        this->_customDisplay = true;
    }

    bool onMenuItemSelected(int menuItemIndex) override { return false; }
    bool onBackPressed() override { return false; }

    bool onBeforeMenuItemSelected(int menuItemIndex) override {
        _menuSystem->changeMenu(_previousMenu);
        return false;
    }
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_ABOUTMENU_H