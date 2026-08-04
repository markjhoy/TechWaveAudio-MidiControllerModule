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

bool Core0Handler::processSignalMessage(SignalCommand command, uint8_t data) {
    switch (command) {
        case SignalCommand_Shutdown_Ack: {
            _waitForAck = false;
            return true;
        }
        case SignalCommand_NoteChange: {
            _runState.currentNote = data;
        } break;
        case SignalCommand_VelocityChange: {
            _runState.currentVelocity = data;
        } break;
        case SignalCommand_Out1Change: {
            _runState.currentOut1 = data;
        } break;
        case SignalCommand_Out2Change: {
            _runState.currentOut2 = data;
        } break;
        case SignalCommand_ClockTick: {
            _runState.clockState = !_runState.clockState;
        } break;
        case SignalCommand_TriggerPulse_On: {
            _runState.triggerState = true;
        } break;
        case SignalCommand_TriggerPulse_Off: {
            _runState.triggerState = false;
        } break;
        case SignalCommand_Gate_On: {
            _runState.gateState = true;
        } break;
        case SignalCommand_Gate_Off: {
            _runState.gateState = false;
        } break;
        case SignalCommand_Reset: {
            _runState = {};
        } break;
        default: {
            return false;
        }
    }
    // ReSharper disable once CppDFANullDereference
    _settingsMenuSystem->setRunningState(&_runState);
    return true;
}

void Core0Handler::onAfterProcessEvents(bool messagesProcessed) {
    // nothing to do
}
