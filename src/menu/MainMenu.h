
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
#include "Out1OutputMenu.h"
#include "Out2OutputMenu.h"
#include "CalibrationMenu.h"
#include "ClockOutputMenu.h"
#include "DisplaySettingsMenu.h"
#include "MidiChannelMenu.h"
#include "NotePriorityMenu.h"
#include "OutputVoltageSelectMenu.h"
#include "RangeEditorMenu.h"
#include "ResetMenu.h"
#include "TriggerDurationMenu.h"
#include "TuningMenu.h"

#define MAIN_MENU_MIDI_CH 0
#define MAIN_MENU_NOTE_PRIORITY 1
#define MAIN_MENU_PITCH_ADJ 2
#define MAIN_MENU_VELOCITY_ADJ 3
#define MAIN_MENU_PITCH_BEND_RANGE 4
#define MAIN_MENU_OUT1_OUTPUT 5
#define MAIN_MENU_OUT2_OUTPUT 6
#define MAIN_MENU_TRIGGER_DUR 7
#define MAIN_MENU_CLOCK_DIVISIONS 8
#define MAIN_MENU_OUTPUT_VOLTAGES 9
#define MAIN_MENU_DISPLAY_OPTIONS 10
#define MAIN_MENU_TUNING 11
#define MAIN_MENU_CALIBRATION 12
#define MAIN_MENU_ABOUT 13
#define MAIN_MENU_RESET_ALL 14
#define MAIN_MENU_NUM_ITEMS 15

class MainMenu : public BaseMenu {
public:
    MainMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState);

    ~MainMenu() override;

    std::string getMenuName() override { return "    Settings"; }

protected:
    void menuInit() override;
    bool onMenuItemSelected(int menuItemIndex) override;
    bool onBackPressed() override;
    bool onBeforeLeftRotation(int currentMenuItemIndex) override;
    bool onBeforeRightRotation(int currentMenuItemIndex) override;

private:
    int _lastMenuItem = 0;
    RangeEditorMenu *_pitchAdjustMenu = nullptr;
    MidiChannelMenu *_midiChannelMenu = nullptr;
    RangeEditorMenu *_pitchBendRangeMenu = nullptr;
    Out1OutputMenu *_out1OutputMenu = nullptr;
    Out2OutputMenu *_out2OutputMenu = nullptr;
    TriggerDurationMenu *_triggerDurationMenu = nullptr;
    ClockOutputMenu *_clockOutputMenu = nullptr;
    RangeEditorMenu *_velocityAdjustMenu = nullptr;
    NotePriorityMenu *_notePriorityMenu = nullptr;
    OutputVoltageSelectMenu *_outputVoltageSelectMenu = nullptr;
    DisplaySettingsMenu *_displaySettingsMenu = nullptr;
    TuningMenu *_tuningMenu = nullptr;
    CalibrationMenu *_calibrationMenu = nullptr;
    AboutMenu *_aboutMenu = nullptr;
    ResetMenu *_resetMenu = nullptr;

    void onPitchAdjustChange(float value) const;
    void onPitchBendAdjustChange(float value) const;
    void onVelocityAdjustChange(float value) const;
    void setupMenus();
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MAINMENU_H