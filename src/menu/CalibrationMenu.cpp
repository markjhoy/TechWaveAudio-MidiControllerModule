
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "CalibrationMenu.h"

#include <sstream>

#include "../Controller.h"
#include "../OutputController.h"
#include "../SettingsMenuSystem.h"
#include "../GlobalHandlers.h"

static std::string diagnostic_menu_selections[] = {
    "Midi in read",
    "Note (100%)",
    "Note (75%)",
    "Note (50%)",
    "Note (25%)",
    "Vel (100%)",
    "Vel (75%)",
    "Vel (50%)",
    "Vel (25%)",
    "Aux (100%)",
    "Aux (75%)",
    "Aux (50%)",
    "Aux (25%)",
    "Ctl (100%)",
    "Ctl (75%)",
    "Ctl (50%)",
    "Ctl (25%)",
    "Pulse gate",
    "Pulse trigger",
    "Pulse clock",
};

CalibrationMenu::CalibrationMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,BaseMenu *previousMenu)
: BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    _midiDiagnosticMenu = new MidiDiagnosticMenu(lcdDisplay, menuSystem, systemState, this);
}

CalibrationMenu::~CalibrationMenu() {
    delete _midiDiagnosticMenu;
}

void CalibrationMenu::init() {
    // shutdown our global output controller
    global_core0_handler->turnOffGlobalOutputController();

    sleep_ms(10);

    // and create our own
    _outputController = new OutputController(_systemState, _menuSystem->getTimerQueue());

    _selectedChoice = 0;
    _noteOutput = _outputController->getNoteOutput();
    _velocityOutput = _outputController->getVelocityOutput();
    _ctlAuxOutput = _outputController->getCtlAuxOutput();

    _wasInitialized = true;

    reset();
}

void CalibrationMenu::display() {
    _lcdDisplay->showMenu("Calibration", diagnostic_menu_selections, _selectedChoice, TOTAL_NUM_CALIBRATION_SELECTIONS);
}

void CalibrationMenu::onEnterPressed() {
    if (_inATest || _closingATest || !_wasInitialized) {
        return;
    }

    switch(_selectedChoice) {
        case CALIBRATION_SELECTION_MIDI_READ: {
            _menuSystem->changeMenu(_midiDiagnosticMenu);
        } break;
        case CALIBRATION_SELECTION_NOTE_FULL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendNoteOutput(100); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_NOTE_THREE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendNoteOutput(75); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_NOTE_HALF: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendNoteOutput(50); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_NOTE_ONE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendNoteOutput(25); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_VELOCITY_FULL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendVelocityOutput(100); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_VELOCITY_THREE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendVelocityOutput(75); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_VELOCITY_HALF: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendVelocityOutput(50); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_VELOCITY_ONE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendVelocityOutput(25); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_AUX_FULL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Aux, 100); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_AUX_THREE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Aux, 75); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_AUX_HALF: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Aux, 50); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_AUX_ONE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Aux, 25); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_CONTROL_FULL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Control, 100); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_CONTROL_THREE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Control, 75); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_CONTROL_HALF: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Control, 50); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_CONTROL_ONE_QTR: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->sendAuxCtlOutput(CVOutput_Control, 25); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_PULSE_TRIGGER: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseTrigger(); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_PULSE_GATE: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseGate(); }, 0);
            return;
        }
        case CALIBRATION_SELECTION_PULSE_CLOCK: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseClock(); }, 0);
            return;
        }
        default: {}
    }
}

void CalibrationMenu::onBackPressed() {
    if (_inATest) {
        _closingATest = true;
        _inATest = false;
        return;
    }
    if (_closingATest) {
        return;
    }

    reset();

    // delete our own OutputController
    _outputController->shutdown();
    delete _outputController;

    // restart the global output controller
    global_core0_handler->turnOnGlobalOutputController();

    _menuSystem->changeMenu(_previousMenu);
}

void CalibrationMenu::onNextPressed() {
    // nothing to do
}

void CalibrationMenu::onUpPressed() {
    if (_inATest || _closingATest || !_wasInitialized) {
        return;
    }

    _selectedChoice--;
    if (_selectedChoice < 0) {
        _selectedChoice = TOTAL_NUM_CALIBRATION_SELECTIONS - 1;
    }
    display();
}

void CalibrationMenu::onDownPressed() {
    if (_inATest || _closingATest || !_wasInitialized) {
        return;
    }

    _selectedChoice++;
    if (_selectedChoice >= TOTAL_NUM_CALIBRATION_SELECTIONS) {
        _selectedChoice = 0;
    }
    display();
}

void CalibrationMenu::reset() {
    if (!_wasInitialized) {
        return;
    }

    _noteOutput->write(0);
    _velocityOutput->write(0);
    _ctlAuxOutput->writeAux(0);
    _ctlAuxOutput->writeCtl(0);

    gpio_put(PIN_NOTE_LED, false);
    gpio_put(PIN_CLOCK_LED, false);
    gpio_put(PIN_TRIGGER_LINE, false);
    gpio_put(PIN_GATE_LINE, false);
    gpio_put(PIN_CLOCK_LINE, false);

    display();

    _inATest = false;
    _closingATest = false;
}

void CalibrationMenu::displayCalibrationScreen(const std::string &testName, const std::string &valueLine) const {
    _lcdDisplay->clear();
    _lcdDisplay->setTitle("[[Calibration]]");
    _lcdDisplay->writeLineAt(2, testName, false);
    _lcdDisplay->writeLineAt(3, valueLine, false);
    _lcdDisplay->show();
}

void CalibrationMenu::testPulseTrigger() {
    _inATest = true;
    displayCalibrationScreen(" pulse trigger", "");
    runPulseTest(PIN_TRIGGER_LINE);
    reset();
}

void CalibrationMenu::testPulseGate() {
    _inATest = true;
    displayCalibrationScreen("   pulse gate", "");
    runPulseTest(PIN_GATE_LINE);
    reset();
}

void CalibrationMenu::testPulseClock() {
    _inATest = true;
    displayCalibrationScreen("  pulse clock", "");
    runPulseTest(PIN_CLOCK_LINE);
    reset();
}

void CalibrationMenu::runPulseTest(int outputPin) {
    while (_inATest) {
        gpio_put(PIN_CLOCK_LED, true);
        gpio_put(PIN_NOTE_LED, false);
        gpio_put(outputPin, true);
        runEventsUntil(DIAGNOSTIC_PULSE_SLEEP_MS);

        if (!_inATest) {
            return;
        }

        gpio_put(PIN_CLOCK_LED, false);
        gpio_put(PIN_NOTE_LED, true);
        gpio_put(outputPin, false);
        runEventsUntil(DIAGNOSTIC_PULSE_SLEEP_MS);
    }
}

void CalibrationMenu::runEventsUntil(uint32_t msExpiration) {
    uint32_t expiration = GetTicksMs + msExpiration;
    TimedEventQueue *timerQueue = _menuSystem->getTimerQueue();
    while (GetTicksMs < expiration && _inATest) {
        timerQueue->pollAndProcessSingleEvent();
    }
}

void CalibrationMenu::sendNoteOutput(int percent) {
    _inATest = true;
    std::stringstream valueText;
    valueText << percent << "%";

    auto cvValue = static_cast<int>((static_cast<float>(percent) / 100.0f) * DAC_4725_MAX_RANGE);
    if (_systemState->noteCVMaxVoltage == FiveVoltOutput) {
        cvValue >>= 1;
        valueText << " (+5v)";
    } else {
        valueText << " (+10v)";
    }
    displayCalibrationScreen("  Note Output", valueText.str());

    _noteOutput->write(cvValue);
    gpio_put(PIN_NOTE_LED, true);
    while (_inATest) {
        tight_loop_contents();
    }
    reset();
}

void CalibrationMenu::sendVelocityOutput(int percent) {
    _inATest = true;
    std::stringstream valueText;
    valueText << percent << "%";

    auto cvValue = static_cast<int>((static_cast<float>(percent) / 100.0f) * DAC_4725_MAX_RANGE);
    if (_systemState->velocityAdjust == FiveVoltOutput) {
        cvValue >>= 1;
        valueText << " (+5v)";
    } else {
        valueText << " (+10v)";
    }

    displayCalibrationScreen("   Vel Output", valueText.str());

    _velocityOutput->write(cvValue);
    gpio_put(PIN_NOTE_LED, true);
    while (_inATest) {
        tight_loop_contents();
    }
    reset();
}

void CalibrationMenu::sendAuxCtlOutput(CVOutput cv_output, int percent) {
    _inATest = true;
    std::stringstream valueText;
    valueText << percent << "%";

    auto cvValue = static_cast<uint>((static_cast<float>(percent) / 100.0f) * DAC_4902_MAX_RANGE);
    if (cv_output == CVOutput_Aux) {
        if (_systemState->auxCVMaxVoltage == FiveVoltOutput) {
            cvValue >>= 1;
            valueText << " (+5v)";
        } else {
            valueText << " (+10v)";
        }

        displayCalibrationScreen("   Aux Output", valueText.str());
        _ctlAuxOutput->writeAux(cvValue);
    } else {
        if (_systemState->ctlCVMaxVoltage == FiveVoltOutput) {
            cvValue >>= 1;
            valueText << " (+5v)";
        } else {
            valueText << " (+10v)";
        }

        displayCalibrationScreen("   Ctl Output", valueText.str());
        _ctlAuxOutput->writeCtl(cvValue);
    }
    gpio_put(PIN_NOTE_LED, true);
    while (_inATest) {
        tight_loop_contents();
    }
    reset();

}
