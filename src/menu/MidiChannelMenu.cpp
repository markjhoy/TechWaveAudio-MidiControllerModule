
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MidiChannelMenu.h"

#include <sstream>

#include "../io/MidiController.h"
#include "../SettingsMenuSystem.h"

extern MidiController *global_midi_controller;

MidiChannelMenu::MidiChannelMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
: BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu)
{}

void MidiChannelMenu::menuInit() {
    std::stringstream temp;
    std::vector<std::string> choices;
    for (int i = 0; i < 17; i++) {
        if (i == 0) {
            choices.emplace_back("all channels");
        } else {
            temp.str(std::string());
            temp << "channel " << i;
            choices.emplace_back(temp.str());
        }
    }
    setMenuItems(choices);
    setCurrentSelectedOption(_systemState->midiChannel);
}

bool MidiChannelMenu::onMenuItemSelected(int menuItemIndex) {
    _systemState->midiChannel = menuItemIndex;
    global_midi_controller->setChannel(menuItemIndex);
    setCurrentSelectedOption(menuItemIndex);
    return true;
}
