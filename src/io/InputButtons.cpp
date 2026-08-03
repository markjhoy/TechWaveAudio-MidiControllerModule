/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "InputButtons.h"

InputButtons::InputButtons() {
    _enterButton = new Button(BUTTON_ENTER_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _enterButton->setOnPressed([this] {
        this->onEnterPressed();
    });

    _backButton = new Button(BUTTON_BACK_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _backButton->setOnPressed([this] {
        this->onBackPressed();
    });

    _nextButton = new Button(BUTTON_NEXT_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _nextButton->setOnPressed([this] {
        this->onNextPressed();
    });

    _upButton = new Button(BUTTON_UP_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _upButton->setOnPressed([this] {
        this->onUpPressed();
    });

    _downButton = new Button(BUTTON_DOWN_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _downButton->setOnPressed([this] {
        this->onDownPressed();
    });
}

InputButtons::~InputButtons() {
    delete _enterButton;
    delete _backButton;
    delete _upButton;
    delete _downButton;
}

void InputButtons::shutdown() {
    delete _enterButton;
    delete _backButton;
    delete _upButton;
    delete _downButton;
    _enterButton = nullptr;
    _backButton = nullptr;
    _upButton = nullptr;
    _downButton = nullptr;
}

void InputButtons::setCallbacks(
    GeneralFunctionCallback enterPressed,
    GeneralFunctionCallback backPressed,
    GeneralFunctionCallback nextPressed,
    GeneralFunctionCallback upPressed,
    GeneralFunctionCallback downPressed
) {
    _enterPressedCallback = enterPressed;
    _backPressedCallback = backPressed;
    _nextPressedCallback = nextPressed;
    _upPressedCallback = upPressed;
    _downPressedCallback = downPressed;
}

void InputButtons::onEnterPressed() {
    if (_enterPressedCallback != nullptr) {
        _enterPressedCallback();
    }
}

void InputButtons::onBackPressed() {
    if (_backPressedCallback != nullptr) {
        _backPressedCallback();
    }
}

void InputButtons::onNextPressed() {
    if (_nextPressedCallback != nullptr) {
        _nextPressedCallback();
    }
}

void InputButtons::onUpPressed() {
    if (_upPressedCallback != nullptr) {
        _upPressedCallback();
    }
}

void InputButtons::onDownPressed() {
    if (_downPressedCallback != nullptr) {
        _downPressedCallback();
    }
}
