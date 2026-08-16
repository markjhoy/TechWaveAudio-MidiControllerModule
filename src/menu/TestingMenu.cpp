
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "TestingMenu.h"

#include <sstream>

#include "../Controller.h"
#include "../io/OutputController.h"
#include "../SettingsMenuSystem.h"
#include "../GlobalHandlers.h"

static std::vector<std::string> diagnostic_menu_selections = {
    "Midi in read",
    "All outputs",
    "Note out",
    "Velocity out",
    "Out 1",
    "Out 2",
    "Pulse gate",
    "Pulse trigger",
    "Pulse clock",
};

static std::vector<std::string> diagnostic_menu_selections_with_ex = {
    "Midi in read",
    "All outputs",
    "Note out",
    "Velocity out",
    "Out 1",
    "Out 2",
    "Out X1",
    "Out X2",
    "Out X3",
    "Out X4",
    "Pulse gate",
    "Pulse trigger",
    "Pulse clock",
};

TestingMenu::TestingMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,BaseMenu *previousMenu, RotaryEncoder *encoder)
: BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    _midiDiagnosticMenu = new MidiDiagnosticMenu(lcdDisplay, menuSystem, systemState, this);
    _encoder = encoder;
}

TestingMenu::~TestingMenu() {
    delete _midiDiagnosticMenu;
}

bool TestingMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    if (_inATest) {
        _closingATest = true;
        _inATest = false;
        return false;
    }
    if (_closingATest) {
        return false;
    }

    return true;
}

bool TestingMenu::onMenuItemSelected(int menuItemIndex) {
    if (_systemState->expansionSensed) {
        return doOnMenuItemSelectedEx(menuItemIndex);
    }

    switch(menuItemIndex) {
        case CALIBRATION_SELECTION_MIDI_READ: {
            _menuSystem->changeMenu(_midiDiagnosticMenu);
        } break;
        case CALIBRATION_SELECTION_TEST_ALL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runAllOutputTest(); }, 0);
        } break;
        case CALIBRATION_SELECTION_NOTE: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Note); }, 0);
        } break;
        case CALIBRATION_SELECTION_VELOCITY: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Velocity); }, 0);
        } break;
        case CALIBRATION_SELECTION_OUT1: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Out1); }, 0);
        } break;
        case CALIBRATION_SELECTION_OUT2: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Out2); }, 0);
        } break;
        case CALIBRATION_SELECTION_PULSE_TRIGGER: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseTrigger(); }, 0);
        } break;
        case CALIBRATION_SELECTION_PULSE_GATE: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseGate(); }, 0);
        } break;
        case CALIBRATION_SELECTION_PULSE_CLOCK: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseClock(); }, 0);
        } break;
        default: {}
    }
    return false;
}

bool TestingMenu::doOnMenuItemSelectedEx(int menuItemIndex) {
    switch(menuItemIndex) {
        case CALIBRATION_SELECTION_MIDI_READ: {
            _menuSystem->changeMenu(_midiDiagnosticMenu);
        } break;
        case CALIBRATION_SELECTION_TEST_ALL: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runAllOutputTest(); }, 0);
        } break;
        case CALIBRATION_SELECTION_NOTE: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Note); }, 0);
        } break;
        case CALIBRATION_SELECTION_VELOCITY: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Velocity); }, 0);
        } break;
        case CALIBRATION_SELECTION_OUT1: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Out1); }, 0);
        } break;
        case CALIBRATION_SELECTION_OUT2: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_Out2); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_OUTX1: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_OutX1); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_OUTX2: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_OutX2); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_OUTX3: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_OutX3); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_OUTX4: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->runCvTest(CVOutput_OutX4); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_PULSE_TRIGGER: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseTrigger(); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_PULSE_GATE: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseGate(); }, 0);
        } break;
        case CALIBRATION_SELECTION_EX_PULSE_CLOCK: {
            _menuSystem->getTimerQueue()->scheduleCallbackEvent([this] { this->testPulseClock(); }, 0);
        } break;
        default: {}
    }
    return false;
}


void TestingMenu::menuInit() {
    if (_systemState->expansionSensed) {
        setMenuItems(diagnostic_menu_selections_with_ex);
    } else {
        setMenuItems(diagnostic_menu_selections);
    }

    // shutdown our global output controller
    global_core0_handler->turnOffGlobalOutputController();

    sleep_ms(10);

    // and create our own
    _outputController = new OutputController(_systemState, _menuSystem->getTimerQueue());
    _outputController->init();
    _outputController->setIgnoreMidi(true);

    _output = _outputController->getNoteVelOut1Out2Output();
    if (_systemState->expansionSensed) {
        _extensionOutput = _outputController->getExtensionOutput();
    }

    setEncoderCallbacksMain();

    _wasInitialized = true;

    reset();
}

bool TestingMenu::onBackPressed() {
    if (_inATest) {
        _closingATest = true;
        _inATest = false;
        return false;
    }
    if (_closingATest) {
        return false;
    }

    // delete our own OutputController
    _outputController->shutdown();
    delete _outputController;

    // restart the global output controller
    global_core0_handler->turnOnGlobalOutputController();

    return true;
}

bool TestingMenu::onBeforeLeftRotation(int currentMenuItemIndex) {
    if (_inATest || _closingATest || !_wasInitialized) {
        return false;
    }
    return true;
}

bool TestingMenu::onBeforeRightRotation(int currentMenuItemIndex) {
    if (_inATest || _closingATest || !_wasInitialized) {
        return false;
    }
    return true;
}

void TestingMenu::reset() {
    if (!_wasInitialized) {
        return;
    }

    gpio_put(PIN_NOTE_LED, false);

    _outputController->reset();
    runEventsUntil(DIAGNOSTIC_PULSE_SLEEP_MS);
    _outputController->setIgnoreMidi(true);

    setEncoderCallbacksMain();

    _inATest = false;
    _closingATest = false;

    display();
}

void TestingMenu::setEncoderCallbacksMain() {
    _encoder->setOnLeftTurn([this] { this->onLeftRotation(); });
    _encoder->setOnRightTurn([this] { this->onRightRotation(); });
    _encoder->setOnPressed([this] { this->onEnterPressed(); });
}

void TestingMenu::displayCalibrationScreen(const std::string &testName, const std::string &valueLine) const {
    _lcdDisplay->clear();
    _lcdDisplay->setTitle(" [[ Testing ]]");
    _lcdDisplay->writeLineAt(2, testName, false);
    _lcdDisplay->writeLineAt(3, valueLine, false);
    _lcdDisplay->show();
}

void TestingMenu::testPulseTrigger() {
    _inATest = true;
    displayCalibrationScreen(" pulse trigger", "");
    runPulseTest(PIN_TRIGGER_LINE);
    reset();
}

void TestingMenu::testPulseGate() {
    _inATest = true;
    displayCalibrationScreen("   pulse gate", "");
    runPulseTest(PIN_GATE_LINE);
    reset();
}

void TestingMenu::testPulseClock() {
    _inATest = true;
    displayCalibrationScreen("  pulse clock", "");
    runPulseTest(PIN_CLOCK_LINE);
    reset();
}

void TestingMenu::runCvTest(CVOutput output) {
    _currentCvTestPercent = 0;
    _inATest = true;

    _encoder->setOnLeftTurn([this, output] {
        _currentCvTestPercent -= 5;
        if (_currentCvTestPercent < 0) {
            _currentCvTestPercent = 0;
        }
        setOutputPercentValue(output);
    });
    _encoder->setOnRightTurn([this, output] {
        _currentCvTestPercent += 5;
        if (_currentCvTestPercent > 100) {
            _currentCvTestPercent = 100;
        }
        setOutputPercentValue(output);
    });
    _encoder->setOnPressed([this] {
        this->_inATest = false;
    });

    setOutputPercentValue(output);

    gpio_put(PIN_NOTE_LED, true);
    TimedEventQueue *timerQueue = _menuSystem->getTimerQueue();
    while (_inATest) {
        timerQueue->pollAndProcessEvents();
    }
    reset();
}

void TestingMenu::setOutputPercentValue(CVOutput output) {
    std::stringstream valueText;
    valueText << _currentCvTestPercent << "%";
    if (output == CVOutput_NONE) {
        displayCalibrationScreen("< all outputs >", valueText.str());
    } else {
        displayCalibrationScreen(CV_OUTPUT_NAME[output], valueText.str());
    }

    auto cvValue = static_cast<int>((static_cast<float>(_currentCvTestPercent) / 100.0f) * DAC_7554_MAX_RANGE);
    switch (output) {
        case CVOutput_NONE: {
            // set value to all the outputs
            _output->writeNote(cvValue);
            _output->writeVelocity(cvValue);
            _output->writeOut1(cvValue);
            _output->writeOut2(cvValue);
            if (_systemState->expansionSensed) {
                _extensionOutput->writeValue(EXT_OUT_X1_REGISTER, cvValue);
                _extensionOutput->writeValue(EXT_OUT_X2_REGISTER, cvValue);
                _extensionOutput->writeValue(EXT_OUT_X3_REGISTER, cvValue);
                _extensionOutput->writeValue(EXT_OUT_X4_REGISTER, cvValue);
            }
        } break;
        case CVOutput_Note: {
            _output->writeNote(cvValue);
        } break;
        case CVOutput_Velocity: {
            _output->writeVelocity(cvValue);
        } break;
        case CVOutput_Out1: {
            _output->writeOut1(cvValue);
        } break;
        case CVOutput_Out2: {
            _output->writeOut2(cvValue);
        } break;
        case CVOutput_OutX1: {
            _extensionOutput->writeValue(EXT_OUT_X1_REGISTER, cvValue);
        } break;
        case CVOutput_OutX2: {
            _extensionOutput->writeValue(EXT_OUT_X2_REGISTER, cvValue);
        } break;
        case CVOutput_OutX3: {
            _extensionOutput->writeValue(EXT_OUT_X3_REGISTER, cvValue);
        } break;
        case CVOutput_OutX4: {
            _extensionOutput->writeValue(EXT_OUT_X4_REGISTER, cvValue);
        } break;
        default: {

        }
    }
}

void TestingMenu::runPulseTest(int outputPin) {
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

void TestingMenu::runAllOutputTest() {
    _currentCvTestPercent = 0;
    _inATest = true;

    _encoder->setOnLeftTurn([this] {
        _currentCvTestPercent -= 5;
        if (_currentCvTestPercent < 0) {
            _currentCvTestPercent = 0;
        }
        setOutputPercentValue(CVOutput_NONE);
    });
    _encoder->setOnRightTurn([this] {
        _currentCvTestPercent += 5;
        if (_currentCvTestPercent > 100) {
            _currentCvTestPercent = 100;
        }
        setOutputPercentValue(CVOutput_NONE);
    });
    _encoder->setOnPressed([this] {
        this->_inATest = false;
    });

    setOutputPercentValue(CVOutput_NONE);

    while (_inATest) {
        gpio_put(PIN_CLOCK_LED, true);
        gpio_put(PIN_NOTE_LED, false);
        gpio_put(PIN_TRIGGER_LINE, true);
        gpio_put(PIN_GATE_LINE, true);
        gpio_put(PIN_CLOCK_LINE, true);
        runEventsUntil(DIAGNOSTIC_PULSE_SLEEP_MS);

        if (!_inATest) {
            break;
        }

        gpio_put(PIN_CLOCK_LED, false);
        gpio_put(PIN_NOTE_LED, true);
        gpio_put(PIN_TRIGGER_LINE, false);
        gpio_put(PIN_GATE_LINE, false);
        gpio_put(PIN_CLOCK_LINE, false);
        runEventsUntil(DIAGNOSTIC_PULSE_SLEEP_MS);
    }
    reset();
}

void TestingMenu::runEventsUntil(uint32_t msExpiration) {
    absolute_time_t expiration = make_timeout_time_ms(msExpiration);
    TimedEventQueue *timerQueue = _menuSystem->getTimerQueue();
    while (get_absolute_time() < expiration && _inATest) {
        timerQueue->pollAndProcessSingleEvent();
    }
}

