
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MENUSYSTEM_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MENUSYSTEM_H

#include <functional>

#include "menu/BaseMenu.h"
#include "display/DashboardDisplay.h"
#include "TechWaveAudio_MidiControllerModule.h"
#include "display/OledDisplay.h"
#include "SystemState.h"
#include "TimedEventQueue.h"
#include "hardware/RotaryEncoder.h"
#include "pico/critical_section.h"

class MainMenu;

/**
 * Our menu system. Controls the menu settings as well as saves and loads any persisted
 * configuration state.
 */
class SettingsMenuSystem {
public:
    SettingsMenuSystem(OledDisplay *lcdDisplay, TimedEventQueue *timerQueue, RotaryEncoder *encoder) __attribute__((nonnull));;
    ~SettingsMenuSystem();

    /**
     * checks if the system is in a menu or the dashboard
     * @return true if the system is in a menu, or false if at the dashboard
     */
    [[nodiscard]] bool isInMenu() const { return _currentMenu != nullptr; }

    /**
     * Sets the callback function to be called whenever the system transitions from
     * the dashboard to the settings menu
     * @param callback the callback function
     */
    inline void setOnEnteringMenu(GeneralFunctionCallback const &callback) {
        _onEnteringMenu = callback;
    }

    /**
     * Sets the callback functiuon to be called when exiting the menu back to the dashboard.
     * @param callback the callback function
     */
    inline void setOnExitingMenu(GeneralFunctionCallback const &callback) {
        _onExitingMenu = callback;
    }

    /**
     * Called when shutting down the controller
     */
    void shutdown();

    /**
     * Sets the current flags for the state of the dashboard
     * Should only be called from the core0 handler
     * @param state the dashboard state to set
     */
    void setRunningState(RunningState_t *state);

    /**
     * Displays the dashboard
     */
    void showDashboard();

    /**
     * Updates the dashboard display
     */
    void updateDashboard(bool midiSensed);

    /**
     * Persists the current settings to flash memory
     */
    void saveState();

    /**
     * Read the latest settings from flash memory
     */
    void loadState();

    /**
     * Resets the state to the defaults. Will save the persisted state if changed.
     */
    void resetState() const;

    /**
     * Displays the main menu
     */
    void showMainMenu();

    /**
     * Switches the system to a new menu.
     * @param newMenu the menu to change to
     */
    void changeMenu(BaseMenu *newMenu);

    /**
     * Retrieves the current timer queue in use for the system
     * @return the timer queue in use
     */
    [[nodiscard]] inline TimedEventQueue *getTimerQueue() const { return _timerQueue; }

    /**
     * Checks to see if we should exit the system.
     * @return true if we should exit, false if not
     */
    [[nodiscard]] inline bool shouldExit() const { return _shouldExit; }
    void setShouldExit() { _shouldExit = true; }

    /**
     * Helper function to check if any state settings have changed.
     * @param initialState the initial state to compare to
     * @return true if any state settings have changed
     */
    bool didStateChange(const SystemState &initialState) const;

private:
    OledDisplay *_lcdDisplay = nullptr;
    TimedEventQueue *_timerQueue = nullptr;
    DashboardDisplay *_dashboardDisplay = nullptr;
    RotaryEncoder *_encoder = nullptr;
    bool _dashboardDot = false;
    critical_section_t _flashLock{};

    GeneralFunctionCallback _onEnteringMenu = nullptr;
    GeneralFunctionCallback _onExitingMenu = nullptr;

    BaseMenu *_currentMenu = nullptr;
    MainMenu *_mainMenu = nullptr;

    uint32_t _nextDashboardUpdate = 0L;
    bool _menuChanged = false;
    bool _shouldExit = false;

    uint8_t *_flashBuffer = nullptr;
    volatile bool _isUpdating = false;

    void changeMenuCallback(BaseMenu * newMenu);
    SystemState readStateFromFlash(int page);
    static uint32_t getStateChecksum(SystemState &state);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MENUSYSTEM_H