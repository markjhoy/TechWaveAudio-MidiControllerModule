
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


SettingsMenuSystem::SettingsMenuSystem(OledDisplay *lcdDisplay, TimedEventQueue *timerQueue, ControllerButtons *buttons) {
    _lcdDisplay = lcdDisplay;
    _timerQueue = timerQueue;
    _buttons = buttons;
    _flashBuffer = new uint8_t[FLASH_PAGE_SIZE];
    critical_section_init(&_flashLock);

    _dashboardDisplay = new DashboardDisplay(_lcdDisplay, global_system_state);
    _mainMenu = new MainMenu(_lcdDisplay, this, global_system_state);
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

void SettingsMenuSystem::setDashboardState(DashboardState_t *state) {
    _dashboardDisplay->setCurrentState(state);
}

void SettingsMenuSystem::showDashboard() {
    DashboardState dashboardState = global_core0_handler->getDashboardState();
    _dashboardDisplay->setCurrentState(&dashboardState);
    _dashboardDisplay->display();
}

void SettingsMenuSystem::updateDashboard() {
    uint32_t now = GetTicksMs;
    if (now < _nextDashboardUpdate) {
        return;
    }

    _dashboardDisplay->update();
    _nextDashboardUpdate = now + global_system_state->dashboardRefreshMs;
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

    critical_section_enter_blocking(&_flashLock);

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

    critical_section_exit(&_flashLock);
}

void SettingsMenuSystem::loadState() {
    if (get_core_num() != 0) {
        // only allow to run on core 0
        return;
    }

    critical_section_enter_blocking(&_flashLock);
    resetState();

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
    critical_section_exit(&_flashLock);
}

void SettingsMenuSystem::resetState() const {
    global_system_state->stateChanged = (
        global_system_state->midiChannel != DEFAULT_MIDI_CHANNEL ||
        global_system_state->pitchAdjust != DEFAULT_PITCH_ADJUST ||
        global_system_state->notePriority != DEFAULT_NOTE_PRIORITY ||
        global_system_state->triggerDuration != DEFAULT_TRIGGER_DURATION ||
        global_system_state->velocityAdjust != DEFAULT_VELOCITY_ADJUST ||
        global_system_state->auxMode != DEFAULT_AUX_SETTING ||
        global_system_state->ctlMode != DEFAULT_CONTROL_SETTING ||
        global_system_state->pitchBendRange != DEFAULT_PITCH_BEND_RANGE_OCTAVES ||
        global_system_state->noteCvOutput != DEFAULT_VOLTS_OUTPUT_NOTE_DAC ||
        global_system_state->velocityCvOutput != DEFAULT_VOLTS_OUTPUT_VELOCITY_DAC ||
        global_system_state->auxCvOutput != DEFAULT_VOLTS_OUTPUT_AUX_DAC ||
        global_system_state->controlCvOutput != DEFAULT_VOLTS_OUTPUT_CTL_DAC
    );
    global_system_state->midiChannel = DEFAULT_MIDI_CHANNEL;
    global_system_state->pitchAdjust = DEFAULT_PITCH_ADJUST;
    global_system_state->notePriority = DEFAULT_NOTE_PRIORITY;
    global_system_state->triggerDuration = DEFAULT_TRIGGER_DURATION;
    global_system_state->velocityAdjust = DEFAULT_VELOCITY_ADJUST;
    global_system_state->auxMode = DEFAULT_AUX_SETTING;
    global_system_state->ctlMode = DEFAULT_CONTROL_SETTING;
    global_system_state->pitchBendRange = DEFAULT_PITCH_BEND_RANGE_OCTAVES;
    global_system_state->noteCvOutput = DEFAULT_VOLTS_OUTPUT_NOTE_DAC;
    global_system_state->velocityCvOutput = DEFAULT_VOLTS_OUTPUT_VELOCITY_DAC;
    global_system_state->auxCvOutput = DEFAULT_VOLTS_OUTPUT_AUX_DAC;
    global_system_state->controlCvOutput = DEFAULT_VOLTS_OUTPUT_CTL_DAC;
}

void SettingsMenuSystem::showMainMenu() {
    global_system_state->stateChanged = false;
    changeMenu(_mainMenu);
}

void SettingsMenuSystem::changeMenu(BaseMenu *newMenu) {
    _timerQueue->scheduleCallbackEvent([this, newMenu]  { changeMenuCallback(newMenu); }, 0);
}

bool SettingsMenuSystem::didStateChange(const SystemState &initialState) const {
    return (
        global_system_state->midiChannel != initialState.midiChannel ||
        global_system_state->pitchAdjust != initialState.pitchAdjust ||
        global_system_state->notePriority != initialState.notePriority ||
        global_system_state->triggerDuration != initialState.triggerDuration ||
        global_system_state->velocityAdjust != initialState.velocityAdjust ||
        global_system_state->auxMode != initialState.auxMode ||
        global_system_state->ctlMode != initialState.ctlMode ||
        global_system_state->pitchBendRange != initialState.pitchBendRange ||
        global_system_state->noteCvOutput != initialState.noteCvOutput ||
        global_system_state->velocityCvOutput != initialState.velocityCvOutput ||
        global_system_state->auxCvOutput != initialState.auxCvOutput ||
        global_system_state->controlCvOutput != initialState.controlCvOutput
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
        _buttons->setCallbacks(
            [this] { _currentMenu->onEnterPressed(); },
            [this] { _currentMenu->onBackPressed(); },
            [this] { _currentMenu->onNextPressed(); },
            [this] { _currentMenu->onUpPressed(); },
            [this] { _currentMenu->onDownPressed(); }
        );
        _currentMenu->init();
        _currentMenu->display();
    } else {
        _buttons->setCallbacks([this] { this->showMainMenu(); }, nullptr, nullptr, nullptr, nullptr);
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
