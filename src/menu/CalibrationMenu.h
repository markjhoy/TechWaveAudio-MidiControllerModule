
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
#include "../OutputController.h"

#define CALIBRATION_SELECTION_MIDI_READ 0
#define CALIBRATION_SELECTION_NOTE_FULL 1
#define CALIBRATION_SELECTION_NOTE_THREE_QTR 2
#define CALIBRATION_SELECTION_NOTE_HALF 3
#define CALIBRATION_SELECTION_NOTE_ONE_QTR 4
#define CALIBRATION_SELECTION_VELOCITY_FULL 5
#define CALIBRATION_SELECTION_VELOCITY_THREE_QTR 6
#define CALIBRATION_SELECTION_VELOCITY_HALF 7
#define CALIBRATION_SELECTION_VELOCITY_ONE_QTR 8
#define CALIBRATION_SELECTION_AUX_FULL 9
#define CALIBRATION_SELECTION_AUX_THREE_QTR 10
#define CALIBRATION_SELECTION_AUX_HALF 11
#define CALIBRATION_SELECTION_AUX_ONE_QTR 12
#define CALIBRATION_SELECTION_CONTROL_FULL 13
#define CALIBRATION_SELECTION_CONTROL_THREE_QTR 14
#define CALIBRATION_SELECTION_CONTROL_HALF 15
#define CALIBRATION_SELECTION_CONTROL_ONE_QTR 16
#define CALIBRATION_SELECTION_PULSE_GATE 17
#define CALIBRATION_SELECTION_PULSE_TRIGGER 18
#define CALIBRATION_SELECTION_PULSE_CLOCK 19
#define TOTAL_NUM_CALIBRATION_SELECTIONS 20

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