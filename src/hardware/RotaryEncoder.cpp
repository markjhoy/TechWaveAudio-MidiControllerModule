
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(TimedEventQueue *timedEventQueue, uint8_t pinA, uint8_t pinB, uint8_t buttonPin) {
    this->_timedEventQueue = timedEventQueue;
    this->_pinA = pinA;
    this->_pinB = pinB;
    this->_buttonPin = buttonPin;
    this->_reverse = false;
    this->setupDevice();
}

RotaryEncoder::RotaryEncoder(TimedEventQueue *timedEventQueue, uint8_t pinA, uint8_t pinB, uint8_t buttonPin, bool reverse) {
    this->_timedEventQueue = timedEventQueue;
    this->_pinA = pinA;
    this->_pinB = pinB;
    this->_buttonPin = buttonPin;
    this->_reverse = reverse;
    this->setupDevice();
}

RotaryEncoder::~RotaryEncoder() {
    delete _leftPinEventHandler;
    delete _rightPinEventHandler;
    delete _buttonEventHandler;
}

void RotaryEncoder::setOnLeftTurn(const std::function<void()> &callback) {
}

void RotaryEncoder::setOnRightTurn(const std::function<void()> &callback) {
}

void RotaryEncoder::setOnPressed(const std::function<void()> &callback) {
}

void RotaryEncoder::setOnReleased(const std::function<void()> &callback) {
}

void RotaryEncoder::setupDevice() {
    _leftPinEventHandler = new GpioPinEventHandler(_pinA);
    _leftPinEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onEncoderDirectionChangeCallback));

    _rightPinEventHandler = new GpioPinEventHandler(_pinB);
    _rightPinEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onEncoderDirectionChangeCallback));

    _buttonEventHandler = new GpioPinEventHandler(_buttonPin);
    _buttonEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onButtonPressedCallback));
}

void RotaryEncoder::onEncoderDirectionChangeCallback(uint8_t pin, bool value) {
    // value is high on rise, low on fall
    uint8_t pinAValue = gpio_get(_pinA) ? 1 : 0;
    uint8_t pinBValue = gpio_get(_pinB) ? 1 : 0;

    uint8_t nextPhaseIndex = (_phaseIndex + 1) % 4;
    uint8_t previousPhaseIndex = (_phaseIndex + 3) % 4;

    uint8_t currentState = (pinBValue << 1) | pinAValue;

    if (currentState == _phaseStates[nextPhaseIndex]) {
        this->_phaseIndex = nextPhaseIndex;
        if (_onRightTurnCallback != nullptr) {
            _onRightTurnCallback();
        }

    } else if (currentState == _phaseStates[previousPhaseIndex]) {
        this->_phaseIndex = previousPhaseIndex;
        if (_onLeftTurnCallback != nullptr) {
            _onLeftTurnCallback();
        }
    }
}

void RotaryEncoder::onButtonPressedCallback(uint8_t pin, bool value) {
    if (pin == this->_buttonPin) {
        if (value && this->_onPressedCallback != nullptr) {
            this->_onPressedCallback();
        } else if (!value && this->_onReleasedCallback != nullptr) {
            this->_onReleasedCallback();
        }
    }
}
