
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H
#include "BaseMenu.h"


class NotePriorityMenu : public BaseMenu {
public:
    NotePriorityMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return " Note Priority"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H