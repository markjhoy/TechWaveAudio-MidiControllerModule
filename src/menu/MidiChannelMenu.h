//
// Created by Mark J. Hoy on 2026-05-03.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H
#include <vector>

#include "BaseMenu.h"


class MidiChannelMenu : public BaseMenu {
public:
    MidiChannelMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu);

    ~MidiChannelMenu() override { _choices.clear(); };

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    inline std::string getMenuName() override { return "Midi Channel"; }
private:
    std::vector<std::string> _choices;
    int _currentSelection = 0;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICHANNELMENU_H