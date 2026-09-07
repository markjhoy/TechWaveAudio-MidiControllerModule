/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/


#ifndef TECHWAVEAUDIO_MCM_V2_0_0_TESTINGMENUSYSTEMHANDLER_H
#define TECHWAVEAUDIO_MCM_V2_0_0_TESTINGMENUSYSTEMHANDLER_H

#include "./common/IMenuSystemHandler.h"
#include "hardware/RotaryEncoder.h"

/**
 * Lightweight menu system handler for our diagnostic testing menu
 */
class TestingMenuSystemHandler : public IMenuSystemHandler {
public:
    TestingMenuSystemHandler(OledDisplay *lcdDisplay, TimedEventQueue *timerQueue, RotaryEncoder *encoder) __attribute__((nonnull));

    ~TestingMenuSystemHandler() override = default;

    [[nodiscard]] bool isInMenu() const override { return _shouldExit == false; }

    void setOnEnteringMenu(const std::function<void()> &callback) override;

    void setOnExitingMenu(const std::function<void()> &callback) override;

    void shutdown() override;

    void changeMenu(BaseMenu *newMenu) override;

    [[nodiscard]] TimedEventQueue *getTimerQueue() const override { return _timerQueue; }

    [[nodiscard]] bool shouldExit() const { return _shouldExit; }

private:
    RotaryEncoder * _encoder = nullptr;
    TimedEventQueue * _timerQueue = nullptr;
    OledDisplay * _lcdDisplay = nullptr;
    BaseMenu * _currentMenu = nullptr;
    volatile bool _isInMenu = false;
    volatile bool _shouldExit = false;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_TESTINGMENUSYSTEMHANDLER_H