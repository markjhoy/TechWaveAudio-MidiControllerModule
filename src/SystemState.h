
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
    /// the voltage range (+5v / +10v) of our Out 2 CV output

    /// our clock tick mapping (valid for _ClockTickX routes)
    volatile OutputMappingRoute clockOutputMapping = DEFAULT_CLOCK_OUT_MAPPING;
    /// mapping for the Out1 CV output
    volatile OutputMappingRoute out1Mapping = DEFAULT_OUT1_MAPPING;
    /// mapping for the Out2 CV output
    volatile OutputMappingRoute out2Mapping = DEFAULT_OUT2_MAPPING;
    /// mapping for the Out x1 CV output
    volatile OutputMappingRoute outX1Mapping = DEFAULT_OUTX1_MAPPING;
    /// mapping for the Out x2 CV output
    volatile OutputMappingRoute outX2Mapping = DEFAULT_OUTX2_MAPPING;
    /// mapping for the Out x3 CV output
    volatile OutputMappingRoute outX3Mapping = DEFAULT_OUTX3_MAPPING;
    /// mapping for the Out x4 CV output
    volatile OutputMappingRoute outX4Mapping = DEFAULT_OUTX4_MAPPING;

    /// the voltage range (+5v / +10v) of our Out 1 CV output
    volatile CVOutputVoltsValue out1CVMaxVoltage = DEFAULT_VOLTS_OUTPUT_OUT1_DAC;
    /// the voltage range (+5v / +10v) of our Out 2 CV output
    volatile CVOutputVoltsValue out2CVMaxVoltage = DEFAULT_VOLTS_OUTPUT_OUT2_DAC;
    /// the voltage range (+5v / +10v) of our Out x1 CV output
    volatile CVOutputVoltsValue outX1Voltage = DEFAULT_VOLTS_OUTPUT_OUTX1_DAC;
    /// the voltage range (+5v / +10v) of our Out x2 CV output
    volatile CVOutputVoltsValue outX2Voltage = DEFAULT_VOLTS_OUTPUT_OUTX2_DAC;
    /// the voltage range (+5v / +10v) of our Out x3 CV output
    volatile CVOutputVoltsValue outX3Voltage = DEFAULT_VOLTS_OUTPUT_OUTX3_DAC;
    /// the voltage range (+5v / +10v) of our Out x4 CV output
    volatile CVOutputVoltsValue outX4Voltage = DEFAULT_VOLTS_OUTPUT_OUTX4_DAC;

    /// flag for displaying the dashboard or not when running
    uint8_t displayDashboard = DEFAULT_SHOW_DASHBOARD;
    /// the number of ms between dashboard display updates
    uint32_t dashboardRefreshMs = DEFAULT_DASHBOARD_UPDATE_FREQUENCY;
    /// the number of cycles for our clock tick LED
    uint8_t clockTickLedCycle = DEFAULT_CLOCK_TICK_LED_CYCLE;

    /// if the expansion module is attached or not
    bool expansionSensed = false;
    /// whether this state object has changed
    bool stateChanged = false;
    /// state validation checksum
    uint32_t checksum = 0x0000;
} SystemState;

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_SYSTEMSTATE_H
