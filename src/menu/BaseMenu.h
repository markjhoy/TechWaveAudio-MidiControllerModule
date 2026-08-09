
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H

#include "../display/OledDisplay.h"
#include "../SystemState.h"
#include "../TechWaveAudio_MidiControllerModule.h"

class SettingsMenuSystem;

#define MENU_BACK_SELECTED_INDEX -1

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

    virtual ~BaseMenu() { _menuItems.clear(); };

    /**
     * Initialize the menu (when entered)
     */
    void init();

    /**
     * Displays the current menu screen
     */
    virtual void display();

    /**
     * Callback when the enter button is pressed
     */
    void onEnterPressed();

    /**
     * Callback when the back button is pressed
     */
    void onLeftRotation();

    /**
     * Callback when the next button is pressed
     */
    void onRightRotation();

    /**
     * Called when the current menu is about to change to another one.
     */
    virtual void onMenuChanging() { }

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
    // pointer to the previous menu
    BaseMenu *_previousMenu = nullptr;
    // set to true if the subclass handles its own display and functionality
    bool _customDisplay = false;
    // the current menu highlight position
    int _currentMenuPosition = 0;

    /**
     * Initializes the subclass menu.
     * The subclass should set up any menu items here, or set the _customDisplay
     * flag if it handles its own display and functions
     */
    virtual void menuInit() = 0;

    /**
     * Called when a menu item is selected
     * Will not be called if _customDisplay is true
     * @param menuItemIndex the 0 based index of the selected item (excluding the << back item)
     * @return true if the display should be refreshed
     */
    virtual bool onMenuItemSelected(int menuItemIndex) = 0;

    /**
     * Called before handling a menu item is selected
     * @param menuItemIndex the 0 based index of the selected item (excluding the << back item)
     * @return false if handling of the selection should stop
     */
    virtual bool onBeforeMenuItemSelected(int menuItemIndex) { return true; }

    /**
     * Called when the encoder is turned left (CCW)
     * @param currentMenuItemIndex the 0 based index of the current selected item before any position change
     * @return true if the menu position should be changed and display refreshed
     */
    virtual bool onBeforeLeftRotation(int currentMenuItemIndex) {
        return true;
    }

    /**
     * Called when the encoder is turned right (CW)
     * @param currentMenuItemIndex the 0 based index of the current selected item before any position change
     * @return true if the menu position should be changed and display refreshed
     */
    virtual bool onBeforeRightRotation(int currentMenuItemIndex) {
        return true;
    }

    /**
     * Called when the user selects the "back" option on the menu.
     * The subclass can override this if functionality is needed before the
     * menu is changed to the previous menu.
     * Will not be called if _customDisplay is true.
     * @return false if the default back functionality should not continue
     */
    virtual bool onBackPressed() {
        return true;
    }

    void setMenuItems(const std::vector<std::string> &menuItems);
    void setMenuItems(const std::string *menuItems, int numItems);
    void changeMenuItem(int index, const std::string &newItem);
    void setCurrentMenuPosition(int index);
    void setCurrentSelectedOption(int index);

private:
    std::vector<std::string> _menuItems{};
    // the currently selected option for the menu
    int _currentSelectedOption = -1;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BASEMENU_H