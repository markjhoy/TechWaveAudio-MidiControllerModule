
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

/**
 * Handler for hardware IRQ GPIO pin event change
 */
class GpioPinEventHandler {
public:
     GpioPinEventHandler(uint8_t pinNumber, const OnPinValueChangeCallback &callback);
    ~GpioPinEventHandler();

     /**
      * Gets the pin number for the handler
      * @return The pin number
      */
     [[nodiscard]] uint8_t getPinNumber() const { return _pinNumber; }

     /**
      * Called by the IRQ handler when the pin state changes
      * @param events the pin state events
      */
     void onPinChange(uint32_t events);

     /**
      * Sets the callback for when the pin state changes
      * @param callback the callback function
      */
     void onPinValueChangeCallback(const OnPinValueChangeCallback &callback) { this->_onPinValueChangeCallback = callback; }
private:
    uint8_t _pinNumber;
    const uint32_t _switchEventMask = GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;
    OnPinValueChangeCallback _onPinValueChangeCallback = nullptr;
    bool _lastActiveState = false;
};

void global_gpio_event_registry_init();

#endif //TECHWAVEAUDIO_MCM_IGPIOPINEVENTHANDLER_H