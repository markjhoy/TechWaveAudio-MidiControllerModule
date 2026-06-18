//
// Created by Mark J. Hoy on 2026-05-24.
//

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H
#include "BaseMenu.h"
#include "DisplayClockLedMenu.h"
#include "DisplayRefreshMenu.h"


class DisplaySettingsMenu : public BaseMenu {
public:
    DisplaySettingsMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {

        _refreshMenu = new DisplayRefreshMenu(lcdDisplay, menuSystem, systemState, this);
        _clockLedMenu = new DisplayClockLedMenu(lcdDisplay, menuSystem, systemState, this);
    }

    ~DisplaySettingsMenu() override {
        delete _refreshMenu;
        delete _clockLedMenu;
    };

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override;

    void onUpPressed() override;

    void onDownPressed() override;

    std::string getMenuName() override { return "Display Settings"; }

private:
    int _currentValue = 0;

    DisplayRefreshMenu *_refreshMenu = nullptr;
    DisplayClockLedMenu *_clockLedMenu = nullptr;
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H