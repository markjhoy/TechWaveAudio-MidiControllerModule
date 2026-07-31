
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H

#include "../TechWaveAudio_MidiControllerModule.h"
#include "../SystemState.h"
#include "OledDisplay.h"

/**
 * Class for displaying up updating the dashboard.
 */
class DashboardDisplay {
public:
    DashboardDisplay(OledDisplay *lcdDisplay, SystemState *systemState) __attribute__((nonnull));
    ~DashboardDisplay();

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
    const ScreenXYPos _posChannelText{0, 0};
    const ScreenXYPos _posChannelValue{112, 0};
    const ScreenRectangle _posNoteArea{0, 16, 64, 48};
    const ScreenRectangle _posNoteName{8, 24, 32, 32};
    const ScreenXYPos _posNoteSharpText{40, 24};
    const ScreenXYPos _posNoteOctave{40, 40};
    const ScreenXYPos _posVelText{64, 16};
    const ScreenRectangle _posVelBar{96,16, 32, 8};
    const ScreenXYPos _posAuxText{64, 24};
    const ScreenRectangle _posAuxBar{96,24, 32, 8};
    const ScreenXYPos _posCtlText{64, 32};
    const ScreenRectangle _posCtlBar{96,32, 32, 8};
    const ScreenRectangle _posTriggerArea{64, 44, 12, 12};
    const ScreenRectangle _posGateArea{80, 44, 12, 12};
    const ScreenRectangle _posClockArea{96, 40, 19, 19};

    OledDisplay *_lcdDisplay;
    SystemState *_systemState;
    ImageSet *_noteDisplayImageSet = nullptr;
    ImageSet *_clockDisplayImageSet = nullptr;

    RunningState_t _currentState {};
    RunningState_t _lastUpdatedState {};
    bool updateInit = false;
    bool _isDashboardCleared = false;
    const float _barWidthPerPartVel = 0.252f; // 32 px wide, max data value at 127
    const float _barWidthPerPartAuxCtl = 0.125f; // 32 px wide / max data value at 255

    void displayMidiChannel();
    void displayNote();
    void displayVelAuxCtl();
    void displayGateTrigger();
    void displayClock();

    void setDefaultTemplate();
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H