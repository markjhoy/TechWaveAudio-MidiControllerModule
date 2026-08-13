
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
    _onLeftTurnCallback = callback;
}

void RotaryEncoder::setOnRightTurn(const std::function<void()> &callback) {
    _onRightTurnCallback = callback;
}

void RotaryEncoder::setOnPressed(const std::function<void()> &callback) {
    _onPressedCallback = callback;
}

void RotaryEncoder::setOnReleased(const std::function<void()> &callback) {
    _onReleasedCallback = callback;
}

void RotaryEncoder::shutdown() {
    _onLeftTurnCallback = nullptr;
    _onRightTurnCallback = nullptr;
    _onPressedCallback = nullptr;
    _onReleasedCallback = nullptr;

    delete _leftPinEventHandler;
    delete _rightPinEventHandler;
    delete _buttonEventHandler;
}

void RotaryEncoder::setupDevice() {
    _leftPinEventHandler = new GpioPinEventHandler(_pinA);
    _leftPinEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onEncoderDirectionChangeCallback));

    _rightPinEventHandler = new GpioPinEventHandler(_pinB);
    _rightPinEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onEncoderDirectionChangeCallback));

    _buttonEventHandler = new GpioPinEventHandler(_buttonPin);
    _buttonEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onButtonPressedCallback));

    _stateSum = 0;
    int pinA = gpio_get(_pinA) ? 1 : 0;
    int pinB = gpio_get(_pinB) ? 1 : 0;
    _state = (pinA << 1) | pinB;
}

void RotaryEncoder::onEncoderDirectionChangeCallback(uint8_t pin, bool value) {
    if (pin != _pinA && pin != _pinB) {
        return;
    }

    int pinA = gpio_get(_pinA) ? 1 : 0;
    int pinB = gpio_get(_pinB) ? 1 : 0;

    _state = ((_state & 0x03) << 2) | (pinA << 1) | pinB;
    _stateSum += STATE_MAP[_state];

    if ((_stateSum % 4) != 0) {
        return;
    }

    if (_stateSum == 4) {
        if (_onLeftTurnCallback) {
            _onLeftTurnCallback();
        }
    } else if (_stateSum == -4) {
        if (_onRightTurnCallback) {
            _onRightTurnCallback();
        }
    }

    _stateSum = 0;
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
