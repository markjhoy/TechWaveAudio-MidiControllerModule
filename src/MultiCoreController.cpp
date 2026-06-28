/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MultiCoreController.h"

bool MultiCoreController::getNextSignal(SignalMessage &message) {
    return queue_try_remove(_inputQueue, &message);
}

void MultiCoreController::sendSignalMessage(const SignalMessage &message) const {
    queue_try_add(_outputQueue, &message);
}
