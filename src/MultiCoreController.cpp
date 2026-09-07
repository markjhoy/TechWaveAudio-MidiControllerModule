/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MultiCoreController.h"

bool MultiCoreController::getNextSignal(SignalMessage &message) const {
    if (queue_is_empty(_inputQueue))
        return false;

    queue_remove_blocking(_inputQueue, &message);
    return true;
}

void MultiCoreController::sendSignalMessage(const SignalMessage &message) const {
    queue_try_add(_outputQueue, &message);
}
