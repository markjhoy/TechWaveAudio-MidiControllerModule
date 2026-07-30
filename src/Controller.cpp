
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "./TechWaveAudio_MidiControllerModule.h"
#include "Controller.h"
#include "GlobalHandlers.h"

#include <cmath>

#include "tusb.h"
#include "tusb_config.h"
#include "common/tusb_types.h"
#include "hardware/PowerSystem.h"
#include "pico/multicore.h"

#define MIDI_NOTE_VALUE_MIDDLE_A 69.0
#define NOTE_FREQUENCY_MIDDLE_A 440.0
#define NOTE_FREQUENCY_C6 1046.50

void initialize_dac_lookup_tables() {
    float linearStepSize12Bit10v = 4096.0f / MAX_NUM_NOTES_10V;
    for (int i = 0; i < MAX_NUM_NOTES_10V; i++) {
        ten_volt_note_12_bit_output[i] = static_cast<uint16_t>(static_cast<float>(i) * linearStepSize12Bit10v);
    }

    float linearStepSize12Bit5v = 4096.0f / MAX_NUM_NOTES_5V;
    for (int i = 0; i < MAX_NUM_NOTES_5V; i++) {
        five_volt_note_12_bit_output[i] = static_cast<uint16_t>(static_cast<float>(i) * linearStepSize12Bit5v);
    }

    float linearStepSize12BitsLinear = 4096.0f / MAX_MIDI_DATA_VALUE;
    for (int i = 0; i < MAX_MIDI_DATA_VALUE; i++) {
        ten_volt_linear_12_bit_output[i] = static_cast<uint16_t>(static_cast<float>(i) * linearStepSize12BitsLinear);
    }

    float linearStepSize68Bit = 256.0f / MAX_MIDI_DATA_VALUE;
    for (int i = 0; i < MAX_MIDI_DATA_VALUE; i++) {
        ten_volt_8_bit_output[i] = static_cast<uint16_t>(static_cast<float>(i) *linearStepSize68Bit);
    }
}

/**
 * Our main core 1 launcher for midi and output handling
 */
void launch_midi_and_output_handler() {
    if (get_core_num() == 0) {
        // only run on core 1
        return;
    }

    tusb_rhport_init_t host_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO
    };
    tusb_init(BOARD_TUD_RHPORT, &host_init);

    global_midi_output_handler->init();

    while (global_midi_output_handler->shouldKeepRunning()) {
        tud_task();
        global_midi_output_handler->processEvents();
    }
}

Controller::Controller() {
    gpio_set_function(OLED_I2C_DATA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(OLED_I2C_CLOCK_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_I2C_DATA_PIN);
    gpio_pull_up(OLED_I2C_CLOCK_PIN);
}

Controller::~Controller() {
    shutdown();

    delete _menuSystem;
    delete _lcdDisplay;
    delete _buttons;
    delete _timerQueue;

    delete _lcdI2c;
}

void Controller::run() {
    // ensure we're on core 0
    if (get_core_num() != 0) {
        return;
    }

    gpio_put(PIN_NOTE_LED, true);
    gpio_put(PIN_CLOCK_LED, true);

    initHardware();

    // ensure that USB is not plugged in
    auto powerSystem = new PowerSystem();
    bool isVSysPower = false;
    if (powerSystem->getPowerSource(&isVSysPower) == PICO_ERROR_NO_DATA || !isVSysPower) {
        // if we have USB power - do not start up
        _lcdDisplay->clear(true);
        _lcdDisplay->powerOff();
        gpio_put(PIN_NOTE_LED, false);
        gpio_put(PIN_CLOCK_LED, false);

        // and wait until we do not have USB power
        while (powerSystem->getPowerSource(&isVSysPower) == PICO_ERROR_NO_DATA || !isVSysPower) {
            tight_loop_contents();
        }

        _lcdDisplay->powerOn();
    }


    // display the boot screen
    showBootSequence();

    initialize_dac_lookup_tables();

    // load persisted state and set menu handlers
    _menuSystem->loadState();
    _menuSystem->setOnEnteringMenu([this] { this->onEnterMenu(); });
    _menuSystem->setOnExitingMenu([this] { this->onExitMenu(); });

    global_core0_handler->init();
    global_core0_handler->setMenuSystem(_menuSystem);

    multicore_reset_core1();
    sleep_ms(50);
    multicore_launch_core1(&launch_midi_and_output_handler);

    // and turn off the boot screen
    completeBootSequence();

    // set our dashboard display
    _menuSystem->changeMenu(nullptr);

    sleep_ms(100);

    // signal to start our output controller on core 1
    global_core0_handler->turnOnGlobalOutputController();

    // main loop
    while (!_menuSystem->shouldExit()) {
        // process any signals from core 1
        global_core0_handler->processEvents();
        // process any events in the timer queue
        _timerQueue->pollAndProcessEvents();

        // if we're in a menu, don't update the dashboard
        if (_menuSystem->isInMenu()) {
            continue;
        }

        // update the dashboard
        _menuSystem->updateDashboard();
    }
    global_core0_handler->shutdown();
}

void Controller::shutdown() const {
    _buttons->shutdown();
    _menuSystem->shutdown();
    _timerQueue->clear();
    _lcdDisplay->clear();
}

void Controller::initHardware() {
    _timerQueue = new TimedEventQueue();
    _buttons = new ControllerButtons();
    _systemState = new SystemState();

    _lcdI2c = new HardwareI2C(&HW_OLED_I2C, OLED_I2C_DATA_PIN, OLED_I2C_CLOCK_PIN, OLED_BUS_HARDWARE_FREQ);
    _lcdDisplay = new OledDisplay(_lcdI2c);

    _menuSystem = new SettingsMenuSystem(_lcdDisplay, _timerQueue, _buttons);
}

void Controller::enterMenuButtonPressed() const {
    _menuSystem->showMainMenu();
}

void Controller::onEnterMenu() {
    _initialState = (*_systemState);
}

void Controller::onExitMenu() {
    _buttons->setCallbacks([this] { this->enterMenuButtonPressed(); }, nullptr, nullptr, nullptr, nullptr);
    if (_menuSystem->didStateChange(_initialState)) {
        _menuSystem->saveState();
    }
    _initialState = (*_systemState);
}

void Controller::showBootSequence() {
    _lcdDisplay->displayBootScreen();
}

void Controller::completeBootSequence() {
    // sanity check with a light pattern to ensure we know
    // we've booted correctly
    sleep_ms(500);
    gpio_put(PIN_NOTE_LED, false);
    gpio_put(PIN_CLOCK_LED, false);
    sleep_ms(250);
    gpio_put(PIN_NOTE_LED, true);
    sleep_ms(250);
    gpio_put(PIN_CLOCK_LED, true);
    sleep_ms(250);
    gpio_put(PIN_NOTE_LED, false);
    sleep_ms(250);
    gpio_put(PIN_CLOCK_LED, false);
}

