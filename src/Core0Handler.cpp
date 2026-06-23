//
// Created by Mark J. Hoy on 2026-06-07.
//

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

void Core0Handler::processSignalMessage(SignalCommand command, uint8_t data) {
    switch (command) {
        case SignalCommand_Shutdown_Ack: {
            _waitForAck = false;
        } break;
        case SignalCommand_NoteChange: {
            _dashboardState.currentNote = data;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_VelocityChange: {
            _dashboardState.currentVelocity = data;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_AuxChange: {
            _dashboardState.currentAux = data;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_ControlChange: {
            _dashboardState.currentCtl = data;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_ClockTick: {
            _dashboardState.clockState = !_dashboardState.clockState;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_TriggerPulse_On: {
            _dashboardState.triggerState = true;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_TriggerPulse_Off: {
            _dashboardState.triggerState = false;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_Gate_On: {
            _dashboardState.gateState = true;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_Gate_Off: {
            _dashboardState.gateState = false;
            _settingsMenuSystem->setDashboardState(&_dashboardState);
        } break;
        case SignalCommand_Reset: {
            _dashboardState = {};
        } break;
        default: {}
    }
}

void Core0Handler::onAfterProcessEvents() {
    // nothing to do
}
