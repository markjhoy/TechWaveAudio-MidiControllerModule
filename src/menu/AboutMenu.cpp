//
// Created by Mark J. Hoy on 2026-05-27.
//

#include "AboutMenu.h"

void AboutMenu::display() {
    _lcdDisplay->clear();

    _lcdDisplay->setTitle(" TechWave Audio");
    _lcdDisplay->writeLineAt(1, "Midi Controller", false);
    _lcdDisplay->writeLineAt(2, TECHWAVEAUDIO_MCM_VERSION_STR, false);
    _lcdDisplay->writeLineAt(3, TECHWAVEAUDIO_MCM_RELEASE_STR, false);

    _lcdDisplay->show();
}
