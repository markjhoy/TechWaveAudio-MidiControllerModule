
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MidiChannelMenu.h"

#include <sstream>

#include "../MidiController.h"
#include "../SettingsMenuSystem.h"

extern MidiController *global_midi_controller;

MidiChannelMenu::MidiChannelMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
: BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu)
{}

void MidiChannelMenu::init() {
    std::stringstream temp;
    _choices.clear();
    for (int i = 0; i < 17; i++) {
        if (i == 0) {
            if (_systemState->midiChannel == 0) {
                _choices.emplace_back("* all channels");
            } else {
                _choices.emplace_back("  all channels");
            }
        } else {
            temp.str(std::string());
            if (_systemState->midiChannel == i) {
                temp << "* channel " << i;
            } else {
                temp << "  channel " << i;
            }
            _choices.emplace_back(temp.str());
        }
    }
}

void MidiChannelMenu::display() {
    _lcdDisplay->showMenu("Midi Channel", _choices.data(), _currentSelection, 17);
}

void MidiChannelMenu::onEnterPressed() {
    _systemState->midiChannel = _currentSelection;
    global_midi_controller->setChannel(_currentSelection);
    init();
    display();
}

void MidiChannelMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void MidiChannelMenu::onUpPressed() {
    _currentSelection -= 1;
    if (_currentSelection < 0) {
        _currentSelection = 0;
    }
    display();
}

void MidiChannelMenu::onDownPressed() {
    _currentSelection += 1;
    if (_currentSelection > 16) {
        _currentSelection = 16;
    }
    display();
}
