
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_IGPIOPINEVENTHANDLER_H
#define TECHWAVEAUDIO_MCM_IGPIOPINEVENTHANDLER_H

#include <cstdint>
#include "../TechWaveAudio_MidiControllerModule.h"
#include "hardware/gpio.h"

class GpioPinEventHandler {
public:
    explicit GpioPinEventHandler(uint8_t pinNumber);
    GpioPinEventHandler(uint8_t pinNumber, uint32_t bounceTimeMs);
    ~GpioPinEventHandler();

    [[nodiscard]] uint8_t getPinNumber() const { return _pinNumber; }
    void onPinChange(uint32_t events);

    void onPinValueChangeCallback(const OnPinValueChangeCallback &callback) { this->_onPinValueChangeCallback = callback; }
private:
    uint8_t _pinNumber;
    const uint32_t _switchEventMask = GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;
    OnPinValueChangeCallback _onPinValueChangeCallback = nullptr;
    bool _lastActiveState = false;
    uint32_t _bounceTimeMs = 0;
    uint32_t _debounceExpirationMs = 0;
};

void global_gpio_event_registry_init();

#endif //TECHWAVEAUDIO_MCM_IGPIOPINEVENTHANDLER_H