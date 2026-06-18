//
// Created by Mark J. Hoy on 2026-05-03.
//

#include "ResetMenu.h"

void ResetMenu::init() {
    _wasReset = false;
}

void ResetMenu::display() {
    _lcdDisplay->clear();
    if (_wasReset) {
        _lcdDisplay->setTitle("Settings Reset");
        _lcdDisplay->writeLineAt(2, " Settings reset ", false);
        _lcdDisplay->writeLineAt(3, "   complete", false);
    } else {
        _lcdDisplay->setTitle("Reset Settings");
        _lcdDisplay->writeLineAt(1, "Confirm reset?", true);
        _lcdDisplay->writeLineAt(2, "[enter] confirm", false);
        _lcdDisplay->writeLineAt(3, " [back] cancel", false);
    }
    _lcdDisplay->show();
}

void ResetMenu::onEnterPressed() {
    if (!_wasReset) {
        _menuSystem->resetState();
        if (_systemState->stateChanged) {
            _menuSystem->saveState();
        }
        _wasReset = true;
        display();
    }
}

void ResetMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}
