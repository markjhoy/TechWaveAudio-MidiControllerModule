
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RESETMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RESETMENU_H
#include "BaseMenu.h"
#include "../SettingsMenuSystem.h"

class ResetMenu : public BaseMenu {
public:
    ResetMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) { }

    ~ResetMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override {}

    void onDownPressed() override {}

    inline std::string getMenuName() override { return "Reset Settings"; }
private:
    bool _wasReset = false;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RESETMENU_H