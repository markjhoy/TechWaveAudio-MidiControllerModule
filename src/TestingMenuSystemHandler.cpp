/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-11.
//

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
        if (newMenu == nullptr) {
            _lcdDisplay->clear(true);
            _shouldExit = true;
            return;
        }

        newMenu->init();
        newMenu->display();
    }, 0);
}
