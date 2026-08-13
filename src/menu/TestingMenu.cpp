
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
    "Note",
    "Velocity",
    "Out1",
    "Out2",
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
    switch(menuItemIndex) {
        case CALIBRATION_SELECTION_MIDI_READ: {
            _menuSystem->changeMenu(_midiDiagnosticMenu);
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

void TestingMenu::menuInit() {
    setMenuItems(diagnostic_menu_selections);

    // shutdown our global output controller
    global_core0_handler->turnOffGlobalOutputController();

    sleep_ms(10);

    // and create our own
    _outputController = new OutputController(_systemState, _menuSystem->getTimerQueue());
    _outputController->init();
    _outputController->setIgnoreMidi(true);

    _output = _outputController->getNoteVelOut1Out2Output();

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

    _outputController->shutdown();
    gpio_put(PIN_NOTE_LED, false);

    this->_menuSystem->getTimerQueue()->scheduleCallbackEvent([this] {
        _outputController->init();
        _outputController->setIgnoreMidi(true);

        setEncoderCallbacksMain();

        display();

        _inATest = false;
        _closingATest = false;
    }, 0);
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
    displayCalibrationScreen(CV_OUTPUT_NAME[output], valueText.str());

    auto cvValue = static_cast<int>((static_cast<float>(_currentCvTestPercent) / 100.0f) * DAC_7554_MAX_RANGE);
    switch (output) {
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

void TestingMenu::runEventsUntil(uint32_t msExpiration) {
    absolute_time_t expiration = make_timeout_time_ms(msExpiration);
    TimedEventQueue *timerQueue = _menuSystem->getTimerQueue();
    while (get_absolute_time() < expiration && _inATest) {
        timerQueue->pollAndProcessSingleEvent();
    }
}

