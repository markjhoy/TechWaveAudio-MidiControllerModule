/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLERBUTTONS_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLERBUTTONS_H

#include "TechWaveAudio_MidiControllerModule.h"
#include "TimedEventQueue.h"
#include "hardware/Button.h"

/**
 * Controller button handler.
 * Processes button initialization and Holds the event callbacks for when pressed.
 */
class ControllerButtons {
public:
    explicit ControllerButtons(TimedEventQueue *timedEventQueue);
    ~ControllerButtons();

    /**
     * Deinitializes the buttons and associated interrupts.
     * Should be called in the main loop shutdown.
     */
    void shutdown() const;

    /**
     * Sets the callback functions for the buttons
     * @param enterPressed callback for the enter button
     * @param backPressed callback for the back button
     * @param nextPressed callback for the next button
     * @param upPressed callback for the up button
     * @param downPressed callback for the down button
     */
    void setCallbacks(
        GeneralFunctionCallback enterPressed,
        GeneralFunctionCallback backPressed,
        GeneralFunctionCallback nextPressed,
        GeneralFunctionCallback upPressed,
        GeneralFunctionCallback downPressed
    );

private:
    TimedEventQueue *_timedEventQueue = nullptr;
    Button *_enterButton = nullptr;
    Button *_backButton = nullptr;
    Button *_nextButton = nullptr;
    Button *_upButton = nullptr;
    Button *_downButton = nullptr;

    GeneralFunctionCallback _enterPressedCallback = nullptr;
    GeneralFunctionCallback _backPressedCallback = nullptr;
    GeneralFunctionCallback _nextPressedCallback = nullptr;
    GeneralFunctionCallback _upPressedCallback = nullptr;
    GeneralFunctionCallback _downPressedCallback = nullptr;

    void onEnterPressed();
    void onBackPressed();
    void onNextPressed();
    void onUpPressed();
    void onDownPressed();
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLLERBUTTONS_H