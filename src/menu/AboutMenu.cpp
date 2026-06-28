
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "AboutMenu.h"

void AboutMenu::display() {
    _lcdDisplay->clear();

    _lcdDisplay->setTitle(" TechWave Audio");
    _lcdDisplay->writeLineAt(1, "Midi Controller", false);
    _lcdDisplay->writeLineAt(2, TECHWAVEAUDIO_MCM_VERSION_STR, false);
    _lcdDisplay->writeLineAt(3, TECHWAVEAUDIO_MCM_RELEASE_STR, false);

    _lcdDisplay->show();
}
