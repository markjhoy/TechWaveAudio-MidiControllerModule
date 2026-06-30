
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H

#include "../OledDisplay.h"
#include "../SystemState.h"
#include "../TechWaveAudio_MidiControllerModule.h"

class SettingsMenuSystem;

/**
 * Base menu class. All menus are derived from this class.
 */
class BaseMenu {
public:
    BaseMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu) {
        _lcdDisplay = lcdDisplay;
        _menuSystem = menuSystem;
        _systemState = systemState;
        _previousMenu = previousMenu;
    }

    virtual ~BaseMenu() = default;

    /**
     * Initialize the menu (when entered)
     */
    virtual void init() = 0;

    /**
     * Displays the current menu screen
     */
    virtual void display() = 0;

    /**
     * Callback when the enter button is pressed
     */
    virtual void onEnterPressed() = 0;

    /**
     * Callback when the back button is pressed
     */
    virtual void onBackPressed() = 0;

    /**
     * Callback when the next button is pressed
     */
    virtual void onNextPressed() = 0;

    /**
     * Callback when the up button is pressed
     */
    virtual void onUpPressed() = 0;

    /**
     * Callback when the down button is pressed
     */
    virtual void onDownPressed() = 0;

    /**
     * Gets the menu name (for debugging)
     * @return the menu name
     */
    virtual std::string getMenuName() = 0;

protected:
    // our display object
    OledDisplay *_lcdDisplay = nullptr;
    // the primary menu system controller
    SettingsMenuSystem *_menuSystem = nullptr;
    // pointer to the system state
    SystemState *_systemState = nullptr;
    // pointer to the previous mennu
    BaseMenu *_previousMenu = nullptr;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H