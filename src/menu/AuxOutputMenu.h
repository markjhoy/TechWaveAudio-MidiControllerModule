//
// Created by Mark J. Hoy on 2026-05-03.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H

#include "BaseMenu.h"

#define MENU_AUX_CONTROL_AFTERTOUCH 0
#define MENU_AUX_CONTROL_MOD_WHEEL 1
#define AUX_CONTROL_MENU_NUM_ITEMS 2

class AuxOutputMenu : public BaseMenu {
public:
    AuxOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
    : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {}

    ~AuxOutputMenu() override = default;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

    inline std::string getMenuName() override { return "Aux Output"; }
private:
    AuxSettingType _currentSelection = AUX_SETTING_AFTERTOUCH;
    AuxSettingType _currentActiveSelection = AUX_SETTING_AFTERTOUCH;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H