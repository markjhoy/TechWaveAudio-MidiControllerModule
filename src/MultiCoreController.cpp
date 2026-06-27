//
// Created by Mark J. Hoy on 2026-06-07.
//

#include "MultiCoreController.h"

bool MultiCoreController::getNextSignal(SignalMessage &message) {
    return queue_try_remove(_inputQueue, &message);
}

void MultiCoreController::sendSignalMessage(const SignalMessage &message) const {
    queue_try_add(_outputQueue, &message);
}
