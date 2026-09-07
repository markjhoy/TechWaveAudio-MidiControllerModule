
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H
#include "BaseMenu.h"

/**
 * Menu to control the rate of the clock LED
 */
class DisplayClockLedMenu : public BaseMenu {
public:
    DisplayClockLedMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return " Clock LED Rate"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

private:
    int _clockRateIndex = 0;
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H