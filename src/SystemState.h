
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SYSTEMSTATE_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SYSTEMSTATE_H
#include "TechWaveAudio_MidiControllerModule.h"
#include "TechWaveAudio_MidiControllerModule.h"

/**
 * Our system state settings structure
 */
typedef struct SystemState_t {
    /// our start marker for validation
    uint16_t stateStart = STATE_START_MARKER;
    /// our state counter (how many times saved, for wear leveling on flash)
    uint32_t stateCounter = STATE_INVALID_PAGE;

    /// the current midi channel listening on
    volatile uint8_t midiChannel = DEFAULT_MIDI_CHANNEL;
    /// any pitch adjustment
    volatile float pitchAdjust = DEFAULT_PITCH_ADJUST;
    /// our note priority settings
    volatile NotePriorityType notePriority = DEFAULT_NOTE_PRIORITY;
    /// the duration in ms for the trigger signal to stay high
    volatile int triggerDuration = DEFAULT_TRIGGER_DURATION;
    /// any adjustment to our velocity CV output
    volatile int velocityAdjust = DEFAULT_VELOCITY_ADJUST;
    /// the number of octaves that our pitch bend will be able to modify
    volatile float pitchBendRange = DEFAULT_PITCH_BEND_RANGE_OCTAVES;

    /// the voltage range (+5v / +10v) of our note CV output
    volatile CVOutputVoltsValue noteCVMaxVoltage = DEFAULT_VOLTS_OUTPUT_NOTE_DAC;
    /// the voltage range (+5v / +10v) of our velocity CV output
    volatile CVOutputVoltsValue velocityCVMaxVoltage = DEFAULT_VOLTS_OUTPUT_VELOCITY_DAC;
    /// the voltage range (+5v / +10v) of our Out 1 CV output
    volatile CVOutputVoltsValue ctlCVMaxVoltage = DEFAULT_VOLTS_OUTPUT_CTL_DAC;
    /// the voltage range (+5v / +10v) of our Out 2 CV output
    volatile CVOutputVoltsValue auxCVMaxVoltage = DEFAULT_VOLTS_OUTPUT_AUX_DAC;

    /// our clock tick mapping (valid for _ClockTickX routes)
    volatile OutputMappingRoute clockOutputMapping = DEFAULT_CLOCK_OUT_MAPPING;
    /// mapping for thr ctl CV
    volatile OutputMappingRoute ctlOutMapping = DEFAULT_CONTROL_MAPPING;
    /// mapping for the aux CV
    volatile OutputMappingRoute auxOutMapping = DEFAULT_AUX_MAPPING;


    /// flag for displaying the dashboard or not when running
    uint8_t displayDashboard = DEFAULT_SHOW_DASHBOARD;
    /// the number of ms between dashboard display updates
    uint32_t dashboardRefreshMs = DEFAULT_DASHBOARD_UPDATE_FREQUENCY;
    /// the number of cycles for our clock tick LED
    uint8_t clockTickLedCycle = DEFAULT_CLOCK_TICK_LED_CYCLE;

    /// whether this state object has changed
    bool stateChanged = false;
    /// state validation checksum
    uint32_t checksum = 0x0000;
} SystemState;

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SYSTEMSTATE_H
