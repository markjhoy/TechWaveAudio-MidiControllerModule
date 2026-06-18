//
// Created by Mark J. Hoy on 2026-05-01.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H

#include "../OledDisplay.h"
#include "../SystemState.h"
#include "../TechWaveAudio_MidiControllerModule.h"

class SettingsMenuSystem;

class BaseMenu {
public:
    BaseMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu) {
        _lcdDisplay = lcdDisplay;
        _menuSystem = menuSystem;
        _systemState = systemState;
        _previousMenu = previousMenu;
        _menuLines = new char[OLED_NUM_CHARS_PER_LINE * OLED_NUM_TEXT_LINES];
    }

    virtual ~BaseMenu() {
        delete _menuLines;
    };

    virtual void init() = 0;
    virtual void display() = 0;
    virtual void onEnterPressed() = 0;
    virtual void onBackPressed() = 0;
    virtual void onNextPressed() = 0;
    virtual void onUpPressed() = 0;
    virtual void onDownPressed() = 0;
    virtual std::string getMenuName() = 0;

protected:
    OledDisplay *_lcdDisplay = nullptr;
    SettingsMenuSystem *_menuSystem = nullptr;
    SystemState *_systemState = nullptr;
    BaseMenu *_previousMenu = nullptr;

    char *_menuLines = nullptr;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H