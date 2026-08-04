
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H

#include <string>
#include "BaseMenu.h"

class DisplayRefreshMenu : public BaseMenu {
public:
    DisplayRefreshMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "Display Refresh"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H