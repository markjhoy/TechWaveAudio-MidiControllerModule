
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
    void update(bool midiSensed);

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
    const ScreenXYPos _posOut1Text{72, 24};
    const ScreenRectangle _posOut1Bar{96,24, 32, 8};
    const ScreenXYPos _posOut2Text{72, 32};
    const ScreenRectangle _posOut2Bar{96,32, 32, 8};
    const ScreenRectangle _posTriggerArea{64, 46, 12, 12};
    const ScreenRectangle _posGateArea{80, 46, 12, 12};
    const ScreenRectangle _posClockArea{96, 42, 19, 19};

    const ScreenXYPos _posExVelText{64, 16};
    const ScreenRectangle _posExVelBar{80,16, 48, 8};
    const ScreenXYPos _posExOut1Text{64, 24};
    const ScreenRectangle _posExOut1Bar{80,24, 16, 8};
    const ScreenXYPos _posExOut2Text{96, 24};
    const ScreenRectangle _posExOut2Bar{112,24, 16, 8};
    const ScreenRectangle _posExTriggerArea{64, 48, 12, 12};
    const ScreenRectangle _posExGateArea{80, 48, 12, 12};
    const ScreenRectangle _posExClockArea{96, 48, 12, 12};

    const ScreenXYPos _posExOutX1Text{64, 32};
    const ScreenRectangle _posExOutX1Bar{80,32, 16, 8};
    const ScreenXYPos _posExOutX2Text{96, 32};
    const ScreenRectangle _posExOutX2Bar{112,32, 16, 8};
    const ScreenXYPos _posExOutX3Text{64, 40};
    const ScreenRectangle _posExOutX3Bar{80,40, 16, 8};
    const ScreenXYPos _posExOutX4Text{96, 40};
    const ScreenRectangle _posExOutX4Bar{112,40, 16, 8};

    ScreenXYPos _setVelText;
    ScreenRectangle _setVelBar;
    ScreenXYPos _setOut1Text;
    ScreenRectangle _setOut1Bar;
    ScreenXYPos _setOut2Text;
    ScreenRectangle _setOut2Bar;
    ScreenRectangle _setTriggerArea;
    ScreenRectangle _setGateArea;
    ScreenRectangle _setClockArea;

    OledDisplay *_lcdDisplay;
    SystemState *_systemState;
    ImageSet *_noteDisplayImageSet = nullptr;
    ImageSet *_clockDisplayImageSet = nullptr;

    RunningState_t _currentState {};
    RunningState_t _lastUpdatedState {};
    bool updateInit = false;
    bool _isDashboardCleared = false;
    float _barWidthPerPartVel;
    float _barWidthPerPartOut1;
    float _barWidthPerPartOut2;
    float _barWidthPerPartXOut;
    bool _lastMidiSenseStatus = false;

    void displayMidiChannel();
    void displayNote();
    void displayVelOut1Out2();
    void displayExOut();
    void displayGateTrigger();
    void displayClock();

    void setDefaultTemplate();
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DASHBOARDDISPLAY_H