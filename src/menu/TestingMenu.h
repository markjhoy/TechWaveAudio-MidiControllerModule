
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

#define CALIBRATION_SELECTION_EXIT 0
#define CALIBRATION_SELECTION_MIDI_READ 0
#define CALIBRATION_SELECTION_TEST_ALL 1
#define CALIBRATION_SELECTION_NOTE 2
#define CALIBRATION_SELECTION_VELOCITY 3
#define CALIBRATION_SELECTION_OUT1 4
#define CALIBRATION_SELECTION_OUT2 5
#define CALIBRATION_SELECTION_PULSE_GATE 6
#define CALIBRATION_SELECTION_PULSE_TRIGGER 7
#define CALIBRATION_SELECTION_PULSE_CLOCK 8

#define CALIBRATION_SELECTION_EX_OUTX1 6
#define CALIBRATION_SELECTION_EX_OUTX2 7
#define CALIBRATION_SELECTION_EX_OUTX3 8
#define CALIBRATION_SELECTION_EX_OUTX4 9
#define CALIBRATION_SELECTION_EX_PULSE_GATE 10
#define CALIBRATION_SELECTION_EX_PULSE_TRIGGER 11
#define CALIBRATION_SELECTION_EX_PULSE_CLOCK 12

class RotaryEncoder;

class TestingMenu : public BaseMenu {
public:
    TestingMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu, RotaryEncoder *encoder);

    ~TestingMenu() override;

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
    Dac7554 *_extensionOutput = nullptr;
    bool _wasInitialized = false;
    volatile bool _inATest = false;
    volatile bool _closingATest = false;
    int _currentCvTestPercent = 0;

    void reset();
    void setEncoderCallbacksMain();

    void displayCalibrationScreen(const std::string &testName, const std::string &valueLine) const;
    bool doOnMenuItemSelectedEx(int menuItemIndex);

    void testPulseTrigger();
    void testPulseGate();
    void testPulseClock();

    void runCvTest(CVOutput output);
    void setOutputPercentValue(CVOutput output);
    void runPulseTest(int outputPin);
    void runAllOutputTest();
    void runEventsUntil(uint32_t msExpiration);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H