//
// Created by Mark J. Hoy on 2026-05-03.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H
#include "BaseMenu.h"


class NotePriorityMenu : public BaseMenu {
public:
    NotePriorityMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    void init() override { _currentSelection = _systemState->notePriority; }

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    inline std::string getMenuName() override { return "Note Priority"; }
private:
    NotePriorityType _currentSelection = NOTE_PRIORITY_LAST;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_NOTEPRIORITYMENU_H