
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

    ~OutputVoltageSelectMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    std::string getMenuName() override { return "Output Voltage Selection"; }
private:
    int _currentSelection = 0;
    std::vector<std::string> _menuItems;

    void setupMenuItems();
};


#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTPUTVOLTAGESELECTMENU_H