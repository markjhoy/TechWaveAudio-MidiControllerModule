
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H

#include "TechWaveAudio_MidiControllerModule.h"
#include "OledDisplay.h"
#include "SystemState.h"

/**
 * Class for displaying up updating the dashboard.
 */
class DashboardDisplay {
public:
    DashboardDisplay(OledDisplay *lcdDisplay, SystemState *systemState);
    ~DashboardDisplay() { delete _displayLines; }

    /**
     * Clears and displays the dashboard
     */
    void display();

    /**
     * Only updates any relevant parts of the dashboard that have changed
     */
    void update();

    /**
     * Sets the current state of the dashboard
     * @param state the dashboard state
     */
    void setCurrentState(RunningState_t *state);

private:
    OledDisplay *_lcdDisplay = nullptr;
    SystemState *_systemState = nullptr;

    char *_displayLines = nullptr;
    RunningState_t _currentState {};
    bool _lastDot = false;
    bool _isDashboardCleared = false;
    const float _barWidthPerPart = 0.1875f; // 3 characters wide / max data value
    BoxSize _textCharSize{};

    void setDefaultTemplate();
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H