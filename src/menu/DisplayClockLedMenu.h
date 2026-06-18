//
// Created by Mark J. Hoy on 2026-05-24.
//

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H
#include "BaseMenu.h"


class DisplayClockLedMenu : public BaseMenu {
public:
    DisplayClockLedMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    ~DisplayClockLedMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    std::string getMenuName() override { return "Display Clock LED Refresh"; }

private:
    int _currentValue = 0;
    uint8_t _selectedValue = 0;
    std::vector<std::string> _choices;

    void setChoices();
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYCLOCKLEDMENU_H