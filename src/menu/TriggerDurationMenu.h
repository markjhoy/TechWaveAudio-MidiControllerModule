
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

    std::string getMenuName() override { return "Trigger Duration"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

private:
    std::vector<std::string> _menuChoices;

    void setChoices();
    static int getDurationIndex(int duration);

};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TRIGGERDURATIONMENU_H