
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include <cmath>
#include <iostream>

#include "Controller.h"
#include "Core0Handler.h"
#include "io/MidiAndOutputHandler.h"
#include "io/MidiController.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/binary_info/code.h"

/**
 * Global variables
 */

// global system state
SystemState *global_system_state;

// our queues for passing messages across the cores
queue_t signal_queue_core_0_read;
queue_t signal_queue_core_1_read;

// our midi handler
MidiAndOutputHandler *global_midi_output_handler;

// handler for core 0 (display and menu)
Core0Handler *global_core0_handler;

// handler for core 1 (midi and output)
MidiController *global_midi_controller;

/**
 * Debugging pin descriptions
 */
void set_declarations() {
    bi_decl(bi_program_description("TechWave Audio MCM-100"));
    bi_decl(bi_1pin_with_name(ONBOARD_LED_PIN, "onboard LED"));
    bi_decl(bi_1pin_with_name(ENC_LEFT_PIN, "Encoder B"));
    bi_decl(bi_1pin_with_name(ENC_RIGHT_PIN, "Encoder A"));
    bi_decl(bi_1pin_with_name(ENC_BUTTON_PIN, "Encoder button"));
    bi_decl(bi_1pin_with_name(OLED_I2C_DATA_PIN, "OLED I2C Data"));
    bi_decl(bi_1pin_with_name(OLED_I2C_CLOCK_PIN, "OLED I2C Clock"));
    bi_decl(bi_1pin_with_name(DAC_7554_SPI_CLOCK_PIN, "quad 12 bit DAC SPI Clock"));
    bi_decl(bi_1pin_with_name(DAC_7554_SPI_TX_PIN, "quad 12 bit DAC SPI TX"));
    bi_decl(bi_1pin_with_name(DAC_7554_SPI_RX_PIN, "quad 12 bit DAC SPI RX"));
    bi_decl(bi_1pin_with_name(DAC_7554_SPI_CS_PIN, "quad 12 bit DAC SPI CS"));
    bi_decl(bi_1pin_with_name(MIDI_OUT_TX_PIN, "midi out TX"));
    bi_decl(bi_1pin_with_name(MIDI_IN_RX_PIN, "midi in RX"));
    bi_decl(bi_1pin_with_name(PIN_CLOCK_LINE, "clock output"));
    bi_decl(bi_1pin_with_name(PIN_TRIGGER_LINE, "trigger output"));
    bi_decl(bi_1pin_with_name(PIN_GATE_LINE, "gate output"));
    bi_decl(bi_1pin_with_name(PIN_NOTE_LED, "note LED"));
    bi_decl(bi_1pin_with_name(PIN_CLOCK_LED, "clock LED"));
    bi_decl(bi_1pin_with_name(PIN_EX_SPI_CLOCK, "extension DAC SPI clock"));
    bi_decl(bi_1pin_with_name(PIN_EX_SPI_TX, "extension DAC SPI TX"));
    bi_decl(bi_1pin_with_name(PIN_EX_SPI_RX, "extension DAC SPI RC"));
    bi_decl(bi_1pin_with_name(PIN_EX_SPI_CS, "extension DAC SPI CS"));
    bi_decl(bi_1pin_with_name(PIN_EX_SENSE, "extension sense line"));
}

/**
 * Initializes a single GPIO pin
 *
 * @param pinId the pin GPIO number
 * @param direction the pin's direction
 * @param pullUp true to pull up (default false)
 * @param pullDown true to pull down (default false)
 */
void initSetupSinglePin(uint pinId, gpio_dir direction, bool pullUp = false, bool pullDown = false) {
    gpio_init(pinId);
    gpio_set_dir(pinId, direction);
    gpio_set_pulls(pinId, pullUp, pullDown);
}

/**
 * Initialize our GPIO pins
 */
void init_all_gpio_pins() {
    initSetupSinglePin(ONBOARD_LED_PIN, GPIO_OUT);

    initSetupSinglePin(ENC_LEFT_PIN, GPIO_IN, true, false);
    initSetupSinglePin(ENC_RIGHT_PIN, GPIO_IN, true, false);
    initSetupSinglePin(ENC_BUTTON_PIN, GPIO_IN);

    initSetupSinglePin(OLED_I2C_DATA_PIN, GPIO_OUT, true, false);
    initSetupSinglePin(OLED_I2C_CLOCK_PIN, GPIO_OUT, true, false);

    initSetupSinglePin(DAC_7554_SPI_CLOCK_PIN, GPIO_OUT);
    initSetupSinglePin(DAC_7554_SPI_TX_PIN, GPIO_OUT);
    initSetupSinglePin(DAC_7554_SPI_RX_PIN, GPIO_IN);
    initSetupSinglePin(DAC_7554_SPI_CS_PIN, GPIO_OUT);

    initSetupSinglePin(MIDI_OUT_TX_PIN, GPIO_OUT);
    initSetupSinglePin(MIDI_IN_RX_PIN, GPIO_IN, true, false);

    initSetupSinglePin(PIN_CLOCK_LINE, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_TRIGGER_LINE, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_GATE_LINE, GPIO_OUT, false, true);

    initSetupSinglePin(PIN_NOTE_LED, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_CLOCK_LED, GPIO_OUT, false, true);

    initSetupSinglePin(PIN_EX_SPI_CLOCK, GPIO_OUT);
    initSetupSinglePin(PIN_EX_SPI_TX, GPIO_OUT);
    initSetupSinglePin(PIN_EX_SPI_RX, GPIO_IN);
    initSetupSinglePin(PIN_EX_SPI_CS, GPIO_OUT);
    initSetupSinglePin(PIN_EX_SENSE, GPIO_IN, true, false);
}

/**
 * Our main program entry point
 *
 * @return (discarded, always 0)
 */
int main() {
    if (get_core_num() != 0) {
        // only run on core 0
        return 0;
    }

    stdio_init_all();
    set_declarations();
    init_all_gpio_pins();

    global_gpio_event_registry_init();

    global_system_state = new SystemState();

    queue_init(&signal_queue_core_0_read, sizeof(SignalMessage_t), MAX_ITEMS_IN_EVENT_QUEUE);
    queue_init(&signal_queue_core_1_read, sizeof(SignalMessage_t), MAX_ITEMS_IN_EVENT_QUEUE);

    global_midi_output_handler = new MidiAndOutputHandler(&signal_queue_core_1_read, &signal_queue_core_0_read);
    global_core0_handler = new Core0Handler(&signal_queue_core_0_read, &signal_queue_core_1_read);
    global_midi_controller = new MidiController(DEFAULT_MIDI_CHANNEL);

    auto controller = new Controller();
    try {
        controller->run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    global_midi_controller->stop();

    controller->shutdown();

    multicore_reset_core1();

    sleep_ms(10);

    delete controller;

    delete global_midi_output_handler;
    delete global_core0_handler;
    delete global_midi_controller;

    queue_free(&signal_queue_core_0_read);
    queue_free(&signal_queue_core_1_read);

    return 0;
}
