
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

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
