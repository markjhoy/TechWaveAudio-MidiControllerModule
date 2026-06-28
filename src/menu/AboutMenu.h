
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
    AboutMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    ~AboutMenu() override = default;

    void init() override { /* nothing to do */ }

    void display() override;

    void onEnterPressed() override  { /* nothing to do */ }

    void onBackPressed() override { _menuSystem->changeMenu(_previousMenu); }

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override { /* nothing to do */ }

    void onDownPressed() override { /* nothing to do */ }

    std::string getMenuName() override { return "About"; }
};


#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_ABOUTMENU_H