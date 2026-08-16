
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include <cstring>
#include <hardware//flash.h>
#include "SettingsMenuSystem.h"
#include "GlobalHandlers.h"
#include "menu/MainMenu.h"


SettingsMenuSystem::SettingsMenuSystem(OledDisplay *lcdDisplay, TimedEventQueue *timerQueue, RotaryEncoder *encoder) {
    _lcdDisplay = lcdDisplay;
    _timerQueue = timerQueue;
    _encoder = encoder;
    _flashBuffer = new uint8_t[FLASH_PAGE_SIZE];
    sem_init(&_flashLock, 1, 1);

    _dashboardDisplay = new DashboardDisplay(_lcdDisplay, global_system_state);
    _mainMenu = new MainMenu(_lcdDisplay, this, global_system_state);
    _nextDashboardUpdate = make_timeout_time_ms(1);
}

SettingsMenuSystem::~SettingsMenuSystem() {
    delete _mainMenu;
    delete _dashboardDisplay;
    delete[] _flashBuffer;
}

void SettingsMenuSystem::shutdown() {
    _lcdDisplay->clear();
    _lcdDisplay->show();
}

void SettingsMenuSystem::setRunningState(RunningState_t *state) {
    _dashboardDisplay->setCurrentState(state);
}

void SettingsMenuSystem::showDashboard() {
    RunningState dashboardState = global_core0_handler->getRunningState();
    _dashboardDisplay->setCurrentState(&dashboardState);
    _dashboardDisplay->display();
}

void SettingsMenuSystem::updateDashboard(bool midiSensed) {
    if (get_absolute_time() < _nextDashboardUpdate) {
        return;
    }

    if (_isUpdating)
        return;

    _isUpdating = true;
    _dashboardDisplay->update(midiSensed);
    _nextDashboardUpdate = make_timeout_time_ms(global_system_state->dashboardRefreshMs);
    _isUpdating = false;
}

static void call_flash_range_erase(void *param) {
    auto offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

static void call_flash_range_program(void *param) {
    uint32_t offset = ((uintptr_t*)param)[0];
    const auto *data = (const uint8_t *)((uintptr_t*)param)[1];
    flash_range_program(offset, data, FLASH_PAGE_SIZE);
}

void SettingsMenuSystem::saveState() {
    if (get_core_num() != 0) {
        // only allow to run on core 0
        return;
    }

    sem_acquire_blocking(&_flashLock);

    // use state counter to find the most recent saved state
    bool wasInvalidPage = false;
    if (global_system_state->stateCounter == STATE_INVALID_PAGE) {
        global_system_state->stateCounter = 0;
        wasInvalidPage = true;
    } else {
        global_system_state->stateCounter++;
    }
    global_system_state->stateChanged = false;

    // set our checksum
    global_system_state->checksum = getStateChecksum(*global_system_state);

    auto pageToSave = global_system_state->stateCounter % (MAX_WEAR_LEVEL_PAGES * 2);
    if (pageToSave >= MAX_WEAR_LEVEL_PAGES || wasInvalidPage) {
        // need to erase
        flash_safe_execute(call_flash_range_erase, (void*)FLASH_TARGET_OFFSET, 10000);
        pageToSave = pageToSave >> 1;
    }

    auto stateSize = sizeof(SystemState);
    SystemState stateToSave = (*global_system_state);
    memset(_flashBuffer, 0, FLASH_PAGE_SIZE);
    memcpy(_flashBuffer, &stateToSave, stateSize);

    uint32_t saveAddress = FLASH_TARGET_OFFSET + (pageToSave * FLASH_PAGE_SIZE);
    uintptr_t programParams[] = { saveAddress, (uintptr_t)_flashBuffer };
    flash_safe_execute(call_flash_range_program, programParams, 10000);

    sem_release(&_flashLock);
}

void SettingsMenuSystem::loadState() {
    if (get_core_num() != 0) {
        // only allow to run on core 0
        return;
    }

    resetState();

    sem_acquire_blocking(&_flashLock);

    // find our state with the highest save counter
    SystemState lastSavedState = SystemState();
    for (int page = 0; page < MAX_WEAR_LEVEL_PAGES; page++) {
        SystemState readState = readStateFromFlash(page);
        if (readState.stateCounter == STATE_INVALID_PAGE) {
            // invalid page
            continue;
        }
        if (lastSavedState.stateCounter != STATE_INVALID_PAGE && readState.stateCounter < lastSavedState.stateCounter) {
            continue;
        }
        lastSavedState = readState;
    }

    *global_system_state = lastSavedState;
    sem_release(&_flashLock);
}

void SettingsMenuSystem::resetState() {
    global_system_state->stateChanged =didStateChange(_defaultState);

    SystemState newState = _defaultState;

    sem_acquire_blocking(&_flashLock);
    newState.stateCounter = global_system_state->stateCounter;
    newState.stateChanged = global_system_state->stateChanged;
    (*global_system_state) = newState;
    sem_release(&_flashLock);
}

void SettingsMenuSystem::showMainMenu() {
    global_system_state->stateChanged = false;
    changeMenu(_mainMenu);
}

void SettingsMenuSystem::changeMenu(BaseMenu *newMenu) {
    _timerQueue->scheduleCallbackEvent([this, newMenu] {
        if (_currentMenu != nullptr)
            _currentMenu->onMenuChanging();

        changeMenuCallback(newMenu);
    }, 0);
}

bool SettingsMenuSystem::didStateChange(const SystemState &initialState) const {
    return (
        global_system_state->midiChannel != initialState.midiChannel ||
        global_system_state->pitchAdjust != initialState.pitchAdjust ||
        global_system_state->notePriority != initialState.notePriority ||
        global_system_state->triggerDuration != initialState.triggerDuration ||
        global_system_state->velocityAdjust != initialState.velocityAdjust ||
        global_system_state->out1Mapping != initialState.out1Mapping ||
        global_system_state->out2Mapping != initialState.out2Mapping ||
        global_system_state->pitchBendRange != initialState.pitchBendRange ||
        global_system_state->noteCVMaxVoltage != initialState.noteCVMaxVoltage ||
        global_system_state->velocityCVMaxVoltage != initialState.velocityCVMaxVoltage ||
        global_system_state->out1CVMaxVoltage != initialState.out1CVMaxVoltage ||
        global_system_state->out2CVMaxVoltage != initialState.out2CVMaxVoltage ||
        global_system_state->clockOutputMapping != initialState.clockOutputMapping ||
        global_system_state->outX1Mapping != initialState.outX1Mapping ||
        global_system_state->outX2Mapping != initialState.outX2Mapping ||
        global_system_state->outX3Mapping != initialState.outX3Mapping ||
        global_system_state->outX4Mapping != initialState.outX4Mapping ||
        global_system_state->outX1Voltage != initialState.outX1Voltage ||
        global_system_state->outX2Voltage != initialState.outX2Voltage ||
        global_system_state->outX3Voltage != initialState.outX3Voltage ||
        global_system_state->outX4Voltage != initialState.outX4Voltage
    );
}

void SettingsMenuSystem::changeMenuCallback(BaseMenu *newMenu) {
    // if we're going from the dashboard to a menu - call our enter menu callback
    if (_currentMenu == nullptr && newMenu != nullptr && _onEnteringMenu != nullptr) {
        _onEnteringMenu();
    }

    BaseMenu *previousMenu = _currentMenu;
    _currentMenu = newMenu;
    if (_currentMenu != nullptr) {
        _encoder->setOnLeftTurn([this] { _currentMenu->onLeftRotation(); });
        _encoder->setOnRightTurn([this] { _currentMenu->onRightRotation(); });
        _encoder->setOnPressed([this] { _currentMenu->onEnterPressed(); });
        _currentMenu->init();
        _currentMenu->display();
    } else {
        _encoder->setOnLeftTurn(nullptr);
        _encoder->setOnRightTurn(nullptr);
        _encoder->setOnPressed([this] { this->showMainMenu(); });

        RunningState dashboardState = global_core0_handler->getRunningState();
        _dashboardDisplay->setCurrentState(&dashboardState);
        _dashboardDisplay->display();

        // if we have an exit menu callback, call it
        if (_onExitingMenu != nullptr && previousMenu != nullptr) {
            _onExitingMenu();
        }
    }
}

SystemState SettingsMenuSystem::readStateFromFlash(int page) {
    auto stateSize = sizeof(SystemState);
    auto memPointer = ((page * FLASH_PAGE_SIZE) + FLASH_TARGET_OFFSET) + XIP_BASE;
    memset(_flashBuffer, 0, FLASH_PAGE_SIZE);
    memcpy(_flashBuffer, reinterpret_cast<uint8_t *>(memPointer), FLASH_PAGE_SIZE);

    SystemState readState = SystemState();
    memcpy(&readState, _flashBuffer, stateSize);

    SystemState invalidState = SystemState();
    invalidState.stateCounter = 0xFFFFFFFF;

    if (readState.stateStart != STATE_START_MARKER) {
        return invalidState;
    }

    auto readChecksum = readState.checksum;

    auto checksum = getStateChecksum(readState);
    if (checksum != readChecksum) {
        return invalidState;
    }

    readState.checksum = checksum;
    readState.stateChanged = false;
    return readState;
}

uint32_t SettingsMenuSystem::getStateChecksum(SystemState &state) {
    state.checksum = 0;
    state.stateChanged = false;

    auto stateSize = sizeof(SystemState);
    auto *asBytes = (uint8_t*)&state;
    uint32_t checksum = 0;
    for (int i = 0; i < stateSize; i++) {
        checksum += asBytes[i];
    }
    return checksum;
}
