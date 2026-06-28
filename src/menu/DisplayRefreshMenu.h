
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H
#include "BaseMenu.h"


class DisplayRefreshMenu : public BaseMenu {
public:
    DisplayRefreshMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    ~DisplayRefreshMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    std::string getMenuName() override { return "Display Refresh"; }

private:
    int _currentValue = 0;
    uint32_t _selectedValue = 0;
    std::vector<std::string> _choices;

    void setChoices();
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYREFRESHMENU_H