//
// Created by Mark J. Hoy on 2026-05-03.
//

#include "MidiController.h"

#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "pico/multicore.h"

extern MidiController *global_midi_controller;

uint8_t global_current_midi_command = 0;
uint8_t global_current_midi_channel = 0xFF;
uint8_t global_current_midi_data_one = 0;
bool global_current_has_midi_data_one = false;
int global_current_midi_expected_bytes = 0;
bool global_read_midi_data_flag = false;
bool global_reading_sys_ex = false;
int global_skip_data_counter = 0;

void reset_global_midi_command_values() {
    global_current_midi_command = 0;
    global_current_midi_channel = 0xFF;
    global_current_midi_data_one = 0;
    global_current_has_midi_data_one = false;
    global_current_midi_expected_bytes = 0;
    global_reading_sys_ex = false;
    global_skip_data_counter = 0;
}

/**
 * Our main handler when incoming data on the UART is ready
 */
void handle_midi_irq_data() {
    while (uart_is_readable(MIDI_UART_ID) && global_midi_controller->isRunning()) {

#ifdef DEBUG_BUILD
        global_read_midi_data_flag = !global_read_midi_data_flag;
        gpio_put(ONBOARD_LED_PIN, global_read_midi_data_flag);
#endif

        uint8_t readValue = uart_getc(MIDI_UART_ID);

        // dp we meed tp skip some data bytes?
        if (global_skip_data_counter > 0) {
            global_skip_data_counter--;
            continue;
        }

        // are we in a SysEx message?
        if (global_reading_sys_ex) {
            if (readValue < 0x80 || readValue >= 0xF0) {
                // still in a SysEx message
                if (readValue == MIDI_CMD_SYSEX_END) {
                    global_reading_sys_ex = false;
                }
                continue;
            }
            // if we're here, it's the start of a new command
            global_reading_sys_ex = false;
        }

        // is it a command packet start?
        if ((readValue & 0x80) > 0) {
            // reset variables
            reset_global_midi_command_values();

            if (readValue == MIDI_CMD_CLOCK_TICK) {
                global_midi_controller->runCommand({
                    0,
                    MIDI_CMD_CLOCK_TICK,
                    0,
                    0
                });
                continue;
            }

            if (readValue == MIDI_CMD_RESET) {
                global_midi_controller->runCommand({
                    0,
                    MIDI_CMD_RESET,
                    0,
                    0
                });
                continue;
            }

            // skip over any SysEx commands
            if (readValue == MIDI_CMD_SYSEX) {
                global_reading_sys_ex = true;
                continue;
            }

            // skip any other system commands
            if (readValue >= 0xF0) {
                if (readValue == MIDI_CMD_TIME_CODE_QTR || readValue == MIDI_CMD_SONG_SELECT) {
                    global_skip_data_counter = 1;
                } else if (readValue == MIDI_CMD_SONG_POSITION) {
                    global_skip_data_counter = 2;
                } else {
                    global_skip_data_counter = 0;
                }
                continue;
            }

            // setup our command to process
            global_current_midi_command = readValue & 0xF0;
            global_current_midi_channel = readValue & 0x0F;
            bool expectingOnlyOneDataByte = (global_current_midi_command == 0xD0 || global_current_midi_command == 0xC0);
            global_current_midi_expected_bytes = expectingOnlyOneDataByte ? 1 : 2;
            continue;
        }

        // if we have the first data byte here, we're expecting the second now
        if (global_current_has_midi_data_one) {
            // we should now have our full message
            global_midi_controller->runCommand({
                global_current_midi_channel,
                global_current_midi_command,
                global_current_midi_data_one,
                readValue
            });
            reset_global_midi_command_values();
            continue;
        }

        // if we are expecting two data bytes, signal this
        if (global_current_midi_expected_bytes == 2) {
            global_current_midi_data_one = readValue;
            global_current_has_midi_data_one = true;
            continue;
        }

        // we have a full message
        global_midi_controller->runCommand({
            global_current_midi_channel,
            global_current_midi_command,
            readValue,
            0
        });
        reset_global_midi_command_values();
    }
}

/**
 * Stops the IRQ for reading incoming UART data
 */
void stop_midi_controller_irq() {
    gpio_put(ONBOARD_LED_PIN, false);

    uart_set_irq_enables(MIDI_UART_ID, false, false);
    irq_set_enabled(MIDI_UART_IRQ, false);
    uart_deinit(MIDI_UART_ID);

    reset_global_midi_command_values();
}

/**
 * Starts the IRQ for reading incoming MIDI data on the UART
 */
void start_midi_controller_irq() {
    gpio_put(ONBOARD_LED_PIN, true);
    uart_init(MIDI_UART_ID, MIDI_BAUD_RATE);
    gpio_set_function(MIDI_IN_RX_PIN, UART_FUNCSEL_NUM(UART_ID, UART_RX_PIN));
    uart_set_hw_flow(MIDI_UART_ID, false, false);
    uart_set_format(MIDI_UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(MIDI_UART_ID, false);
    irq_set_exclusive_handler(MIDI_UART_IRQ, handle_midi_irq_data);

    reset_global_midi_command_values();
    irq_set_enabled(MIDI_UART_IRQ, true);
    uart_set_irq_enables(MIDI_UART_ID, true, false);
}

MidiController::MidiController(uint8_t channel) {
    _midiChannel = channel;
    _isStarted = false;
    _isPaused = false;
}

MidiController::~MidiController() {
    stop();
}

void MidiController::start() {
    if (_isStarted) {
        return;
    }

    _isStarted = true;
    _isPaused = false;

    start_midi_controller_irq();
}

void MidiController::stop() {
    if (!_isStarted) {
        return;
    }

    pause();

    _isStarted = false;
    stop_midi_controller_irq();
}

void MidiController::setChannel(int newChannel) {
    if (newChannel < 0 || newChannel > 16) {
        return;
    }

    pause();

    if (_midiChannel != newChannel) {
        // if we're switching channels, reset
        if (_onResetCallback) { _onResetCallback(); }
    }
    _midiChannel = newChannel;

    resume();
}

void MidiController::pause() {
    _isPaused = true;
}

void MidiController::resume() {
    _isPaused = false;
}

void MidiController::setMute(bool mute) {
    _muteAll = mute;
}

void MidiController::runCommand(const MidiMessage &message) const {
    if (_isPaused || !_isStarted) {
        // drop message
        return;
    }

    if (message.command == MIDI_CMD_CLOCK_TICK) {
        if (_onClockCallback) { _onClockCallback(); }
        return;
    }

    if (message.command == MIDI_CMD_RESET) {
        // reset
        if (_onResetCallback) { _onResetCallback(); }
        return;
    }

    // are we muted?
    if (_muteAll) {
        // drop message
        return;
    }

    // check to see what channel we got this message on
    if (_midiChannel > 0 && message.channel != 0 && message.channel != _midiChannel) {
        // drop message
        return;
    }

    switch (message.command) {
        case MIDI_CMD_NOTE_OFF: {
            // note off
            if (_onNoteOffCallback) { _onNoteOffCallback(message.data1, message.data2); }
        } break;
        case MIDI_CMD_NOTE_ON: {
            // note on
            if (_onNoteOnCallback) { _onNoteOnCallback(message.data1, message.data2); }
        } break;
        case MIDI_CMD_POLY_AFTERTOUCH: {
            // poly aftertouch - not implemented
        } break;
        case MIDI_CMD_CONTROL_CHANGE: {
            // control change
            switch (message.data1) {
                case MIDI_CMD_CONTROL_CHANGE_MOD_WHEEL: {
                    // mod wheel
                    if (_onModWheelCallback) { _onModWheelCallback(message.data2);}
                } break;
                case MIDI_CMD_CONTROL_CHANGE_MOD_SUSTAIN: {
                    // sustain
                    if (_onSustainCallback) { _onSustainCallback(message.data2); }
                } break;
                case MIDI_CMD_CONTROL_CHANGE_MOD_VOLUME: {
                    // volume
                    if (_onVolumeChangeCallback) { _onVolumeChangeCallback(message.data2); }
                } break;
                case MIDI_CMD_CONTROL_CHANGE_MOD_MUTE: {
                    // all notes off
                    if (_onAllNotesOffCallback) { _onAllNotesOffCallback(); }
                } break;
                case MIDI_CMD_CONTROL_CHANGE_EXPRESSION: {
                    // expression pedal
                    if (_onExpressionCallback) { _onExpressionCallback(message.data2); }
                } break;
                case MIDI_CMD_CONTROL_CHANGE_EFFECT_1: {
                    // effect one
                    if (_onEffectOneCallback) { _onEffectOneCallback(message.data2); }
                } break;
                case MIDI_CMD_CONTROL_CHANGE_EFFECT_2: {
                    // effect two
                    if (_onEffectTwoCallback) { _onEffectTwoCallback(message.data2); }
                } break;
                default: {}
            }
        } break;
        case MIDI_CMD_PROGRAM_CHANGE: {
                // program change - not implemented
        } break;
        case MIDI_CMD_CHANNEL_AFTERTOUCH: {
            // aftertouch
            if (_onAftertouchCallback) { _onAftertouchCallback(message.data1); }
        } break;
        case MIDI_CMD_PITCH_BEND: {
            // pitch bend
            if (_onPitchBendCallback) { _onPitchBendCallback(message.data1, message.data2); }
        } break;
        default: {}
    }
}
