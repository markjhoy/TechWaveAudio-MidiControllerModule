//
// Created by Mark J. Hoy on 2026-04-30.
//

#include "ControllerButtons.h"

ControllerButtons::ControllerButtons(TimedEventQueue *timedEventQueue) {
    _timedEventQueue = timedEventQueue;
    global_button_registry_init();

    _enterButton = new Button(_timedEventQueue, BUTTON_ENTER_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _enterButton->setOnPressed([this] {
        this->onEnterPressed();
    });

    _backButton = new Button(_timedEventQueue, BUTTON_BACK_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _backButton->setOnPressed([this] {
        this->onBackPressed();
    });

    _nextButton = new Button(_timedEventQueue, BUTTON_NEXT_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _nextButton->setOnPressed([this] {
        this->onNextPressed();
    });

    _upButton = new Button(_timedEventQueue, BUTTON_UP_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _upButton->setOnPressed([this] {
        this->onUpPressed();
    });

    _downButton = new Button(_timedEventQueue, BUTTON_DOWN_PIN, BTN_PIN_BOUNCE_TIME_MS);
    _downButton->setOnPressed([this] {
        this->onDownPressed();
    });
}

ControllerButtons::~ControllerButtons() {
    delete _enterButton;
    delete _backButton;
    delete _upButton;
    delete _downButton;
}

void ControllerButtons::shutdown() const {
    _enterButton->shutdown();
    _backButton->shutdown();
    _upButton->shutdown();
    _downButton->shutdown();
}

void ControllerButtons::setCallbacks(
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

void ControllerButtons::onEnterPressed() {
    if (_enterPressedCallback != nullptr) {
        _enterPressedCallback();
    }
}

void ControllerButtons::onBackPressed() {
    if (_backPressedCallback != nullptr) {
        _backPressedCallback();
    }
}

void ControllerButtons::onNextPressed() {
    if (_nextPressedCallback != nullptr) {
        _nextPressedCallback();
    }
}

void ControllerButtons::onUpPressed() {
    if (_upPressedCallback != nullptr) {
        _upPressedCallback();
    }
}

void ControllerButtons::onDownPressed() {
    if (_downPressedCallback != nullptr) {
        _downPressedCallback();
    }
}
