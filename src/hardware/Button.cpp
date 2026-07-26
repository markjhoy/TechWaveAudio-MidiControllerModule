
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Button.h"

#include <functional>


Button::Button(uint8_t pinNumber) {
    setupButton(pinNumber, 0L);
}

Button::Button(uint8_t pinNumber, uint32_t bounceTime) {
    setupButton(pinNumber, bounceTime);
}

void Button::setOnPressed(const GeneralFunctionCallback &callback) {
    this->_onPressedCallback = callback;
}

void Button::setOnReleased(const GeneralFunctionCallback &callback) {
    this->_onReleasedCallback = callback;
}

void Button::setupButton(int pinNumber, uint32_t bounceTime) {
    this->_pinNumber = pinNumber;
    _buttonEventHandler = new GpioPinEventHandler(pinNumber, bounceTime);
    _buttonEventHandler->onPinValueChangeCallback(FxnDoubleValueCallback(this->onButtonPressed));
}

void Button::onButtonPressed(uint8_t pin, bool value) {
    if (pin != this->_pinNumber)
        return;

    if (value && _onPressedCallback != nullptr) {
        _onPressedCallback();
    } else if (!value && _onReleasedCallback != nullptr) {
        _onReleasedCallback();
    }
}
