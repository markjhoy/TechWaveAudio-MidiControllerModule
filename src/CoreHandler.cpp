
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "Core0Handler.h"

bool CoreHandler::processEvents() {
    int numEventsRead = 0;
    bool messageProcessed = false;

    SignalMessage message;
    while (_multiCoreController->getNextSignal(message) && numEventsRead < MAX_MESSAGE_EVENTS_TO_PROCESS) {
        numEventsRead++;
        if (this->processSignalMessage(message.command, message.data))
            messageProcessed = true;
    }

    onAfterProcessEvents(messageProcessed);
    return messageProcessed;
}

void CoreHandler::sendSignal(SignalCommand command, uint8_t data) const {
    _multiCoreController->sendSignalMessage({ command, data});
}
