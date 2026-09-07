
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

/**
 * Our menu to reset to default settings
 */
class ResetMenu : public BaseMenu {
public:
    ResetMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) { }

    void display() override;

    std::string getMenuName() override { return "Reset Settings"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

    bool onBeforeLeftRotation(int currentMenuItemIndex) override;

    bool onBeforeRightRotation(int currentMenuItemIndex) override;

private:
    bool _wasReset = false;
    bool _selectReset = false;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RESETMENU_H