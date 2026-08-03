
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H
#include <atomic>

#include "BaseMenu.h"
#include "MidiDiagnosticMenu.h"
#include "../io/OutputController.h"

#define CALIBRATION_SELECTION_MIDI_READ 0
#define CALIBRATION_SELECTION_NOTE_FULL 1
#define CALIBRATION_SELECTION_NOTE_HALF 2
#define CALIBRATION_SELECTION_VELOCITY_FULL 3
#define CALIBRATION_SELECTION_VELOCITY_HALF 4
#define CALIBRATION_SELECTION_AUX_FULL 5
#define CALIBRATION_SELECTION_AUX_HALF 6
#define CALIBRATION_SELECTION_CONTROL_FULL 7
#define CALIBRATION_SELECTION_CONTROL_HALF 8
#define CALIBRATION_SELECTION_PULSE_GATE 9
#define CALIBRATION_SELECTION_PULSE_TRIGGER 10
#define CALIBRATION_SELECTION_PULSE_CLOCK 11
#define TOTAL_NUM_CALIBRATION_SELECTIONS 12

class CtlAuxDacOutput;
class Mcp4725;

class CalibrationMenu : public BaseMenu {
public:
    CalibrationMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu);

    ~CalibrationMenu() override;

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override;

    void onUpPressed() override;

    void onDownPressed() override;

    [[nodiscard]] inline std::string getMenuName() override { return "Diagnostic Menu"; }

private:
    OutputController *_outputController = nullptr;
    MidiDiagnosticMenu *_midiDiagnosticMenu = nullptr;
    int _selectedChoice = 0;
    Mcp4725 * _noteOutput = nullptr;
    Mcp4725 * _velocityOutput = nullptr;
    CtlAuxDacOutput * _ctlAuxOutput = nullptr;
    bool _wasInitialized = false;
    std::atomic<bool> _inATest = false;
    std::atomic<bool> _closingATest = false;

    void reset();
    void displayCalibrationScreen(const std::string &testName, const std::string &valueLine) const;

    void testPulseTrigger();
    void testPulseGate();
    void testPulseClock();

    void runPulseTest(int outputPin);
    void runEventsUntil(uint32_t msExpiration);

    void sendNoteOutput(int percent);
    void sendVelocityOutput(int percent);
    void sendAuxCtlOutput(CVOutput cv_output, int percent);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_DIAGNOSTICMENU_H