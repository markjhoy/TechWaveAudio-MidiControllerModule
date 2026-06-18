#include <cmath>
#include <iostream>

#include "Controller.h"
#include "Core0Handler.h"
#include "MidiController.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/binary_info/code.h"

void set_declarations() {
    bi_decl(bi_program_description("Midi to CV Controller"));
    bi_decl(bi_1pin_with_name(ONBOARD_LED_PIN, "onboard LED"));
    bi_decl(bi_1pin_with_name(BUTTON_BACK_PIN, "back button"));
    bi_decl(bi_1pin_with_name(BUTTON_ENTER_PIN, "enter button"));
    bi_decl(bi_1pin_with_name(BUTTON_UP_PIN, "up button"));
    bi_decl(bi_1pin_with_name(BUTTON_DOWN_PIN, "down button"));
    bi_decl(bi_1pin_with_name(OLED_I2C_DATA_PIN, "OLED I2C Data"));
    bi_decl(bi_1pin_with_name(OLED_I2C_CLOCK_PIN, "OLED I2C Clock"));
    bi_decl(bi_1pin_with_name(DAC_4725_I2C_DATA_PIN, "12 bit DAC I2C Data"));
    bi_decl(bi_1pin_with_name(DAC_4725_I2C_CLOCK_PIN, "12 bit DAC I2C Clock"));
    bi_decl(bi_1pin_with_name(DAC_4902_SPI_CLOCK_PIN, "8 bit DAC SPI Clock"));
    bi_decl(bi_1pin_with_name(DAC_4902_SPI_TX_PIN, "8 bit DAC SPI TX"));
    bi_decl(bi_1pin_with_name(DAC_4902_SPI_RX_PIN, "8 bit DAC SPI RX"));
    bi_decl(bi_1pin_with_name(DAC_4902_SPI_CS_PIN, "8 bit DAC SPI CS"));
    bi_decl(bi_1pin_with_name(MIDI_OUT_TX_PIN, "midi out TX"));
    bi_decl(bi_1pin_with_name(MIDI_IN_RX_PIN, "midi in RX"));
    bi_decl(bi_1pin_with_name(PIN_CLOCK_LINE, "clock output"));
    bi_decl(bi_1pin_with_name(PIN_TRIGGER_LINE, "trigger output"));
    bi_decl(bi_1pin_with_name(PIN_GATE_LINE, "gate output"));
    bi_decl(bi_1pin_with_name(PIN_NOTE_LED, "note LED"));
    bi_decl(bi_1pin_with_name(PIN_CLOCK_LED, "clock LED"));
}

void initSetupSinglePin(uint pinId, gpio_dir direction, bool pullUp = false, bool pullDown = false) {
    gpio_init(pinId);
    gpio_set_dir(pinId, direction);
    gpio_set_pulls(pinId, pullUp, pullDown);
}

void init_all_gpio_pins() {
    initSetupSinglePin(ONBOARD_LED_PIN, GPIO_OUT);

    initSetupSinglePin(BUTTON_BACK_PIN, GPIO_IN, true, false);
    initSetupSinglePin(BUTTON_ENTER_PIN, GPIO_IN, true, false);
    initSetupSinglePin(BUTTON_UP_PIN, GPIO_IN, true, false);
    initSetupSinglePin(BUTTON_DOWN_PIN, GPIO_IN, true, false);

    initSetupSinglePin(OLED_I2C_DATA_PIN, GPIO_OUT, true, false);
    initSetupSinglePin(OLED_I2C_CLOCK_PIN, GPIO_OUT, true, false);
    initSetupSinglePin(DAC_4725_I2C_DATA_PIN, GPIO_OUT, true, false);
    initSetupSinglePin(DAC_4725_I2C_CLOCK_PIN, GPIO_OUT, true, false);

    initSetupSinglePin(DAC_4902_SPI_CLOCK_PIN, GPIO_OUT);
    initSetupSinglePin(DAC_4902_SPI_TX_PIN, GPIO_OUT);
    initSetupSinglePin(DAC_4902_SPI_RX_PIN, GPIO_IN);
    initSetupSinglePin(DAC_4902_SPI_CS_PIN, GPIO_OUT);

    initSetupSinglePin(MIDI_OUT_TX_PIN, GPIO_OUT);
    initSetupSinglePin(MIDI_IN_RX_PIN, GPIO_IN);

    initSetupSinglePin(PIN_CLOCK_LINE, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_TRIGGER_LINE, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_GATE_LINE, GPIO_OUT, false, true);

    initSetupSinglePin(PIN_NOTE_LED, GPIO_OUT, false, true);
    initSetupSinglePin(PIN_CLOCK_LED, GPIO_OUT, false, true);
}

SystemState *global_system_state = new SystemState();
queue_t signal_queue_core_0_read;
queue_t signal_queue_core_1_read;
MidiAndOutputHandler *global_midi_output_handler = new MidiAndOutputHandler(&signal_queue_core_1_read, &signal_queue_core_0_read);
Core0Handler *global_core0_handler = new Core0Handler(&signal_queue_core_0_read, &signal_queue_core_1_read);
MidiController *global_midi_controller = new MidiController(DEFAULT_MIDI_CHANNEL);

void launch_midi_and_output_handler() {
    if (get_core_num() == 0) {
        // only run on core 1
        return;
    }

    gpio_put(ONBOARD_LED_PIN, true);

    while (global_midi_output_handler->shouldKeepRunning()) {
        global_midi_output_handler->processEvents();
    }
}

int main() {
    if (get_core_num() != 0) {
        // only run on core 0
        return 0;
    }

    stdio_init_all();
    set_declarations();
    init_all_gpio_pins();

    queue_init(&signal_queue_core_0_read, sizeof(SignalCommand), MAX_SIGNALS_IN_QUEUE);
    queue_init(&signal_queue_core_1_read, sizeof(SignalCommand), MAX_SIGNALS_IN_QUEUE);

    multicore_reset_core1();
    sleep_ms(10);
    multicore_launch_core1(&launch_midi_and_output_handler);

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

    return 0;
}
