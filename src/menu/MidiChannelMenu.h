
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H

#include "BaseMenu.h"


class MidiChannelMenu : public BaseMenu {
public:
    MidiChannelMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu);



    inline std::string getMenuName() override { return "  Midi Channel"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H