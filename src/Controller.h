/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLER_H

#include "TimedEventQueue.h"
#include "ControllerButtons.h"
#include "display/OledDisplay.h"
#include "io/MidiAndOutputHandler.h"
#include "SettingsMenuSystem.h"
#include "pico/util/queue.h"

/**
 * Primary controller for the application.
 * Handles running the main loop, event handling, and shutdown
 */
class Controller {
public:
    Controller();
    ~Controller();

    /**
     * Run the main loop
     */
    void run();

    /**
     * Call this to shut down all processes.
     * Should be called after the main loop has exited.
     */
    void shutdown() const;

private:
    HardwareI2C *_lcdI2c = nullptr;
    OledDisplay *_lcdDisplay = nullptr;
    TimedEventQueue *_timerQueue = nullptr;
    ControllerButtons * _buttons = nullptr;
    SystemState *_systemState = nullptr;
    SettingsMenuSystem *_menuSystem = nullptr;
    SystemState _initialState;

    void initHardware();
    void enterMenuButtonPressed() const;
    void onEnterMenu();
    void onExitMenu();
    void showBootSequence();
    static void completeBootSequence();
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLER_H