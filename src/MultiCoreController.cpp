/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MultiCoreController.h"

bool MultiCoreController::getNextSignal(SignalMessage &message) {
    uint32_t status = save_and_disable_interrupts();
    bool retVal = queue_try_remove(_inputQueue, &message);
    restore_interrupts(status);
    return retVal;
}

void MultiCoreController::sendSignalMessage(const SignalMessage &message) const {
    uint32_t status = save_and_disable_interrupts();
    queue_try_add(_outputQueue, &message);
    restore_interrupts(status);
}
