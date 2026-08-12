/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_INPUTSETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_INPUTSETTINGSMENU_H
#include "BaseMenu.h"
#include "MidiChannelMenu.h"
#include "NotePriorityMenu.h"


class InputSettingsMenu : public BaseMenu {
public:
    InputSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu)
    {
        _midiChannelMenu = new MidiChannelMenu(_lcdDisplay,_menuSystem,_systemState,this);
        _notePriorityMenu = new NotePriorityMenu(_lcdDisplay,_menuSystem,_systemState,this);
    }

    ~InputSettingsMenu() override {
        delete _midiChannelMenu;
        delete _notePriorityMenu;
    }

    std::string getMenuName() override { return " Input Settings"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

private:
    MidiChannelMenu *_midiChannelMenu = nullptr;
    NotePriorityMenu *_notePriorityMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_INPUTSETTINGSMENU_H