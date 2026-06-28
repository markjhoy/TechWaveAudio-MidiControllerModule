
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TRIGGERDURATIONMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TRIGGERDURATIONMENU_H
#include <vector>

#include "BaseMenu.h"

#define TOTAL_NUM_TRIGGER_DURATIONS 12

class TriggerDurationMenu : public BaseMenu {
public:
    TriggerDurationMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
    : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) { }

    ~TriggerDurationMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    inline std::string getMenuName() override { return "Trigger Duration"; }
private:
    int _currentMenuItem = 0;
    std::vector<std::string> _choices;

    void setChoices();
    static int getDurationIndex(int duration);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TRIGGERDURATIONMENU_H