/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_GLOBALHANDLERS_H
#define TECHWAVEAUDIO_MCM_GLOBALHANDLERS_H

#include "Core0Handler.h"
#include "MidiAndOutputHandler.h"

// our global system state
extern SystemState *global_system_state;

// our handler for the midi and output processing (core 1)
extern MidiAndOutputHandler *global_midi_output_handler;

// our handler for the user input, menu, and display handling (core 0)
extern Core0Handler *global_core0_handler;

#endif //TECHWAVEAUDIO_MCM_GLOBALHANDLERS_H