
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_POWERSYSTEM_H
#define TECHWAVEAUDIO_MCM_POWERSYSTEM_H
#include "pico/error.h"
#include "../TechWaveAudio_MidiControllerModule.h"
#include "hardware/gpio.h"

class PowerSystem {
public:
    static int getPowerSource(bool *vSysPower) {
#if defined PICO_VBUS_PIN
        gpio_set_function(PICO_VBUS_PIN, GPIO_FUNC_SIO);
        *vSysPower = !gpio_get(PICO_VBUS_PIN);
        return PICO_OK;
#else
    return PICO_ERROR_NO_DATA;
#endif
    }
};


#endif //TECHWAVEAUDIO_MCM_POWERSYSTEM_H