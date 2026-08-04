
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTVOLTAGESELECTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTVOLTAGESELECTMENU_H
#include "BaseMenu.h"


class OutputVoltageSelectMenu : public BaseMenu {
public:
    OutputVoltageSelectMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "  CV Voltages"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

private:
    std::vector<std::string> _menuChoices;

    void setupMenuChoices();
};


#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTVOLTAGESELECTMENU_H