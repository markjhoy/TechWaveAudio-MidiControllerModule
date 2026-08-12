
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MAINMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MAINMENU_H
#include "BaseMenu.h"
#include "AboutMenu.h"
#include "OutputRouteSettingsMenu.h"
#include "Out1OutputMenu.h"
#include "Out2OutputMenu.h"
#include "OutX1OutputMenu.h"
#include "OutX2OutputMenu.h"
#include "OutX3OutputMenu.h"
#include "OutX4OutputMenu.h"
#include "CalibrationMenu.h"
#include "ClockOutputMenu.h"
#include "DisplaySettingsMenu.h"
#include "InputSettingsMenu.h"
#include "MidiChannelMenu.h"
#include "NotePriorityMenu.h"
#include "OutputSettingsMenu.h"
#include "OutputVoltageSelectMenu.h"
#include "RangeEditorMenu.h"
#include "ResetMenu.h"
#include "TriggerDurationMenu.h"
#include "TuningMenu.h"

#define MAIN_MENU_OUTPUT_ROUTING 0
#define MAIN_MENU_INPUT_SETTINGS 1
#define MAIN_MENU_OUTPUT_SETTINGS 2
#define MAIN_MENU_DISPLAY_SETTINGS 3
#define MAIN_MENU_UTILITIES 4
#define MAIN_MENU_ABOUT 5
#define MAIN_MENU_RESET_ALL 6
#define MAIN_MENU_NUM_ITEMS 7

/*
 * New menu items:
 *
 * View Output Routing (Output Routings)
 * Input Settings
 * Output Settings
 * Display Settings
 * Utilities
 * About
 * Reset
 */

class MainMenu : public BaseMenu {
public:
    MainMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState);

    ~MainMenu() override;

    std::string getMenuName() override { return "    Settings"; }

protected:
    void menuInit() override;
    bool onMenuItemSelected(int menuItemIndex) override;
    bool onBeforeLeftRotation(int currentMenuItemIndex) override;
    bool onBeforeRightRotation(int currentMenuItemIndex) override;

private:
    int _lastMenuItem = 0;
    OutputRouteSettingsMenu *_outputRouteSettingsMenu = nullptr;
    InputSettingsMenu *_inputSettingsMenu = nullptr;
    OutputSettingsMenu *_outputSettingsMenu = nullptr;
    DisplaySettingsMenu *_displaySettingsMenu = nullptr;
    TuningMenu *_tuningMenu = nullptr;
    AboutMenu *_aboutMenu = nullptr;
    ResetMenu *_resetMenu = nullptr;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MAINMENU_H