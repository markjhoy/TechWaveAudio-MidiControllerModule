/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "TestingMenuSystemHandler.h"

TestingMenuSystemHandler::TestingMenuSystemHandler(OledDisplay *lcdDisplay, TimedEventQueue *timerQueue,
    RotaryEncoder *encoder) {
    this->_lcdDisplay = lcdDisplay;
    this->_timerQueue = timerQueue;
    this->_encoder = encoder;
}

void TestingMenuSystemHandler::setOnEnteringMenu(const std::function<void()> &callback) {
}

void TestingMenuSystemHandler::setOnExitingMenu(const std::function<void()> &callback) {
}

void TestingMenuSystemHandler::shutdown() {
    _lcdDisplay->clear(true);
    _encoder->setOnLeftTurn(nullptr);
    _encoder->setOnRightTurn(nullptr);
    _encoder->setOnPressed(nullptr);
}

void TestingMenuSystemHandler::changeMenu(BaseMenu *newMenu) {
    _timerQueue->scheduleCallbackEvent([this, newMenu] {
        _currentMenu = newMenu;

        if (_currentMenu == nullptr) {
            _lcdDisplay->clear(true);
            _encoder->setOnLeftTurn(nullptr);
            _encoder->setOnRightTurn(nullptr);
            _encoder->setOnPressed(nullptr);
            _shouldExit = true;
            return;
        }

        _encoder->setOnLeftTurn([this] { _currentMenu->onLeftRotation(); });
        _encoder->setOnRightTurn([this] { _currentMenu->onRightRotation(); });
        _encoder->setOnPressed([this] { _currentMenu->onEnterPressed(); });
        _currentMenu->init();
        _currentMenu->display();
    }, 0);
}
