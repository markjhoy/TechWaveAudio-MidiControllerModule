/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_IMENUSYSTEMHANDLER_H
#define TECHWAVEAUDIO_MCM_V2_0_0_IMENUSYSTEMHANDLER_H

#include "TimedEventQueue.h"
#include "../TechWaveAudio_MidiControllerModule.h"
#include "../menu//BaseMenu.h"

/**
 * Base interface for a menu system object
 */
class IMenuSystemHandler {
public:
    virtual ~IMenuSystemHandler() = default;

    /**
     * checks if the system is in a menu or the dashboard
     * @return true if the system is in a menu, or false if at the dashboard
     */
    virtual bool isInMenu() const = 0;

    /**
     * Sets the callback function to be called whenever the system transitions from
     * the dashboard to the settings menu
     * @param callback the callback function
     */
    virtual void setOnEnteringMenu(GeneralFunctionCallback const &callback) = 0;

    /**
     * Sets the callback functiuon to be called when exiting the menu back to the dashboard.
     * @param callback the callback function
     */
    virtual void setOnExitingMenu(GeneralFunctionCallback const &callback) = 0;

    /**
     * Called when shutting down the menu system
     */
    virtual void shutdown() = 0;

    /**
     * Gets the timer queue
     * @return The timer queue used in this handler
     */
    [[nodiscard]] virtual TimedEventQueue *getTimerQueue() const = 0;

    /**
     * Switches the system to a new menu.
     * @param newMenu the menu to change to
     */
    virtual void changeMenu(BaseMenu *newMenu) = 0;

};

#endif //TECHWAVEAUDIO_MCM_V2_0_0_IMENUSYSTEMHANDLER_H