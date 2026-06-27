//
// Created by Mark J. Hoy on 2026-06-07.
//

#include "Core0Handler.h"

void CoreHandler::processEvents() {
    int numMessagesProcessed = 0;
    SignalMessage message;
    while (_multiCoreController->getNextSignal(message) && numMessagesProcessed < MAX_MESSAGE_EVENTS_TO_PROCESS) {
        this->processSignalMessage(message.command, message.data);
        numMessagesProcessed++;
    }
    onAfterProcessEvents();
}

void CoreHandler::sendSignal(SignalCommand command, uint8_t data) const {
    _multiCoreController->sendSignalMessage({ command, data});
}
