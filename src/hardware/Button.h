
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H

#include "../TechWaveAudio_MidiControllerModule.h"
#include "../TimedEventQueue.h"
#include "hardware/gpio.h"

/**
 * A class for handling hardware buttons.
 * Provisions are made for both hardware and software debouncing.
 *
 * All buttons are held in a global registry that is used by the hardware IRQ
 * to detect signal change events. Assumes the signal goes low when pressed
 * (contact is connected to ground)
 *
 * When a button is pressed or released, and a callback function is available,
 * an callback event is placed in the event queue to process the signal.
 */
class Button {
public:
    Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber);
    Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber, uint32_t bounceTime);

    ~Button() = default;

    /**
     * Sets the function to callback when the button is pressed
     * @param callback the callback function
     */
    void setOnPressed(const GeneralFunctionCallback &callback);

    /**
     * Sets the function to callback when the button is released
     * @param callback the callback function
     */
    void setOnReleased(const GeneralFunctionCallback &callback);

    /**
     * Gets the GPIO pin number for the button
     * @return the pin number associated with the button
     */
    [[nodiscard]] uint8_t getPinNumber() const { return _pinNumber; }

    /**
     * Callback function from the IRQ that listens for button signal change events
     * @param events the events triggered
     */
    void onButtonPressed(uint32_t events);

    /**
     * Shuts down and deregisteres the button
     */
    void shutdown() const;

private:
    TimedEventQueue *_timedEventQueue = nullptr;
    uint8_t _pinNumber = 0;

    GeneralFunctionCallback _onPressedCallback = nullptr;
    GeneralFunctionCallback _onReleasedCallback = nullptr;
    const uint32_t _switchEventMask = GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;

#ifndef USE_HARDWARE_DEBOUNCE
    bool _lastActiveState = false;
    uint32_t _bounceTimeMs = 0L;
    uint32_t _debounceExpirationMs = 0;
#endif

    void setupButton(int pinNumber, uint32_t bounceTime);
};

void global_button_registry_init();

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H