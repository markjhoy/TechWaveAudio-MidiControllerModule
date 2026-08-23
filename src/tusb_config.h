/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_TUSB_CONFIG_H
#define TECHWAVEAUDIO_MCM_TUSB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// USB Settings configuration. You should not need to edit these
//--------------------------------------------------------------------

// defined by compiler flags for flexibility
#define CFG_TUSB_MCU OPT_MCU_RP2040

#define CFG_TUD_ENABLED 1
#define BOARD_TUD_RHPORT 0

#ifndef CFG_TUD_MEM_SECTION
#define CFG_TUD_MEM_SECTION
#endif

#ifndef CFG_TUD_MEM_ALIGN
#define CFG_TUD_MEM_ALIGN     __attribute__ ((aligned(4)))
#endif

#ifndef BOARD_TUD_MAX_SPEED
#define BOARD_TUD_MAX_SPEED   OPT_MODE_DEFAULT_SPEED
#endif

#define CFG_TUD_MAX_SPEED BOARD_TUD_MAX_SPEED

#define CFG_TUD_MIDI              1
#define CFG_TUD_MIDI_RX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_MIDI_TX_BUFSIZE   (TUD_OPT_HIGH_SPEED ? 512 : 64)

#define CFG_TUD_ENUMERATION_BUFSIZE 256

#ifdef CFG_TUD_RPI_PIO_USB
#undef CFG_TUD_RPI_PIO_USB
#endif

#ifdef __cplusplus
}
#endif

#endif //TECHWAVEAUDIO_MCM_TUSB_CONFIG_H