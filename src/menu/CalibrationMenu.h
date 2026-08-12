
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H

#include "BaseMenu.h"
#include "MidiDiagnosticMenu.h"
#include "../io/OutputController.h"

#define CALIBRATION_SELECTION_MIDI_READ 0
#define CALIBRATION_SELECTION_NOTE 1
#define CALIBRATION_SELECTION_VELOCITY 2
#define CALIBRATION_SELECTION_OUT1 3
#define CALIBRATION_SELECTION_OUT2 4
#define CALIBRATION_SELECTION_PULSE_GATE 5
#define CALIBRATION_SELECTION_PULSE_TRIGGER 6
#define CALIBRATION_SELECTION_PULSE_CLOCK 7
#define TOTAL_NUM_CALIBRATION_SELECTIONS 8

class RotaryEncoder;

class CalibrationMenu : public BaseMenu {
public:
    CalibrationMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu, RotaryEncoder *encoder);

    ~CalibrationMenu() override;

    void display() override;

    [[nodiscard]] inline std::string getMenuName() override { return "   Test Menu"; }

protected:
    void menuInit() override;

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

    bool onMenuItemSelected(int menuItemIndex) override;

    bool onBackPressed() override;

    bool onBeforeLeftRotation(int currentMenuItemIndex) override;

    bool onBeforeRightRotation(int currentMenuItemIndex) override;

private:
    OutputController *_outputController = nullptr;
    MidiDiagnosticMenu *_midiDiagnosticMenu = nullptr;
    RotaryEncoder *_encoder = nullptr;
    int _selectedChoice = 0;
    NoteVelOut1Out2Output *_output = nullptr;
    bool _wasInitialized = false;
    std::atomic<bool> _inATest = false;
    std::atomic<bool> _closingATest = false;
    int _currentCvTestPercent = 0;

    void reset();
    void setEncoderCallbacksMain();

    void displayCalibrationScreen(const std::string &testName, const std::string &valueLine) const;

    void testPulseTrigger();
    void testPulseGate();
    void testPulseClock();

    void runCvTest(CVOutput output);
    void setOutputPercentValue(CVOutput output);
    void runPulseTest(int outputPin);
    void runEventsUntil(uint32_t msExpiration);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H