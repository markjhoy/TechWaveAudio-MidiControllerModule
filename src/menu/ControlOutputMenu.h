
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
#include "BaseMenu.h"


class ControlOutputMenu : public BaseMenu {
public:
    ControlOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
    : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {}

    ~ControlOutputMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    inline std::string getMenuName() override { return "Control Output"; }
private:
    ControlSettingType _currentSelection = CTL_SETTING_MOD_WHEEL;
    ControlSettingType _currentActiveSelection = CTL_SETTING_MOD_WHEEL;

};


#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H