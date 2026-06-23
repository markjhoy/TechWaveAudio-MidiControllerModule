//
// Created by Mark J. Hoy on 2026-06-07.
//

#include "Core0Handler.h"

void CoreHandler::processEvents(int maxEvents) {
    int eventsProcessed = 0;
    SignalMessage message;
    while (_multiCoreController->getNextSignal(message) && (maxEvents == 0 || (eventsProcessed >= maxEvents))) {
        this->processSignalMessage(message.command, message.data);
        eventsProcessed++;
    }
    onAfterProcessEvents();
}

void CoreHandler::sendSignal(SignalCommand command, uint8_t data) const {
    _multiCoreController->sendSignalMessage({ command, data});
}
