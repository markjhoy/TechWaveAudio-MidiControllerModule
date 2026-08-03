
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_ROTARTYENCODER_H
#define TECHWAVEAUDIO_MCM_ROTARTYENCODER_H

#include "GpioPinEventHandler.h"
#include "../TechWaveAudio_MidiControllerModule.h"
#include "../TimedEventQueue.h"
#include "hardware/gpio.h"


class RotaryEncoder {
public:
    RotaryEncoder(TimedEventQueue *timedEventQueue, uint8_t pinA, uint8_t pinB, uint8_t buttonPin);
    RotaryEncoder(TimedEventQueue *timedEventQueue, uint8_t pinA, uint8_t pinB, uint8_t buttonPin, bool reverse);
    ~RotaryEncoder();

    /**
     * Sets the function to callback when the encoder is turned left 1 position
     * @param callback the callback function
     */
    void setOnLeftTurn(const GeneralFunctionCallback &callback);

    /**
     * Sets the function to callback when the encoder is turned right 1 position
     * @param callback the callback function
     */
    void setOnRightTurn(const GeneralFunctionCallback &callback);

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


private:
    TimedEventQueue *_timedEventQueue = nullptr;
    uint8_t _pinA = 0;
    uint8_t _pinB = 0;
    uint8_t _buttonPin = 0;
    uint8_t _phaseIndex = 0;
    bool _reverse = false;

    GpioPinEventHandler *_leftPinEventHandler = nullptr;
    GpioPinEventHandler *_rightPinEventHandler = nullptr;
    GpioPinEventHandler *_buttonEventHandler = nullptr;

    GeneralFunctionCallback _onLeftTurnCallback = nullptr;
    GeneralFunctionCallback _onRightTurnCallback = nullptr;
    GeneralFunctionCallback _onPressedCallback = nullptr;
    GeneralFunctionCallback _onReleasedCallback = nullptr;
    const uint8_t _phaseStates[4] =
    {
        0b00,
        0b01,
        0b11,
        0b10
    };

#ifndef USE_HARDWARE_DEBOUNCE
    bool _lastActiveState = false;
    uint32_t _bounceTimeMs = 0L;
    uint32_t _debounceExpirationMs = 0;
#endif

    void setupDevice();
    void onEncoderDirectionChangeCallback(uint8_t pin, bool value);
    void onButtonPressedCallback(uint8_t pin, bool value);
};


#endif //TECHWAVEAUDIO_MCM_ROTARTYENCODER_H