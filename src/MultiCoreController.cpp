//
// Created by Mark J. Hoy on 2026-06-07.
//

#include "MultiCoreController.h"

bool MultiCoreController::getNextSignal(SignalMessage &message) {
    if (queue_is_empty(_inputQueue))
        return false;
    queue_remove_blocking(_inputQueue, &message);
    return true;
}

void MultiCoreController::sendSignalMessage(const SignalMessage &message) const {
    queue_add_blocking(_outputQueue, &message);
}
