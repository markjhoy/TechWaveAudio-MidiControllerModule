/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Core0Handler.h"

void Core0Handler::shutdown() {
    _waitForAck = true;
    sendSignal(SignalCommand_Shutdown, 0);
    while (_waitForAck) {
        processEvents();
    }
}

void Core0Handler::turnOffGlobalOutputController() const {
    sendSignal(SignalCommand_OutputOff, 0);
}

void Core0Handler::turnOnGlobalOutputController() const {
    sendSignal(SignalCommand_OutputOn, 0);
}

void Core0Handler::sendRouteMappingUpdateSignal() const {
    sendSignal(SignalCommand_UpdateMappings, 0);
}

void Core0Handler::processSignalMessage(SignalCommand command, uint8_t data) {
    switch (command) {
        case SignalCommand_Shutdown_Ack: {
            _waitForAck = false;
        } break;
        case SignalCommand_NoteChange: {
            _runState.currentNote = data;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_VelocityChange: {
            _runState.currentVelocity = data;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_AuxChange: {
            _runState.currentAux = data;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_ControlChange: {
            _runState.currentCtl = data;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_ClockTick: {
            _runState.clockState = !_runState.clockState;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_TriggerPulse_On: {
            _runState.triggerState = true;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_TriggerPulse_Off: {
            _runState.triggerState = false;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_Gate_On: {
            _runState.gateState = true;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_Gate_Off: {
            _runState.gateState = false;
            _settingsMenuSystem->setRunningState(&_runState);
        } break;
        case SignalCommand_Reset: {
            _runState = {};
        } break;
        default: {}
    }
}

void Core0Handler::onAfterProcessEvents() {
    // nothing to do
}
