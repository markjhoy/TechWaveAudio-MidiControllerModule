
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H

#include "GpioPinEventHandler.h"
#include "../TechWaveAudio_MidiControllerModule.h"

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
    explicit Button(uint8_t pinNumber);
    Button(uint8_t pinNumber, uint32_t bounceTime);

    ~Button() {
        delete _buttonEventHandler;
    };

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

private:
    uint8_t _pinNumber = 0;
    GpioPinEventHandler *_buttonEventHandler = nullptr;

    GeneralFunctionCallback _onPressedCallback = nullptr;
    GeneralFunctionCallback _onReleasedCallback = nullptr;

    void onButtonPressed(uint8_t pin, bool value);

    void setupButton(int pinNumber, uint32_t bounceTime);
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H