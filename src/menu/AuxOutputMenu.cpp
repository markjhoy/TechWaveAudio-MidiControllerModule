
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "AuxOutputMenu.h"
#include "../SettingsMenuSystem.h"
#include <sstream>

static std::string menu_aux_output_choices[] = {
    "Aftertouch",
    "Expression"
};


void AuxOutputMenu::init() {
    _currentSelection = AUX_SETTING_AFTERTOUCH;
    _currentActiveSelection = _systemState->auxMode;
}

void AuxOutputMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->showMenu("Aux Output", menu_aux_output_choices, _currentSelection, 2, _currentActiveSelection);
    _lcdDisplay->show();
}

void AuxOutputMenu::onEnterPressed() {
    _systemState->auxMode = _currentSelection;
    display();
}

void AuxOutputMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void AuxOutputMenu::onUpPressed() {
    switch (_currentSelection) {
        case AUX_SETTING_AFTERTOUCH: break;
        case AUX_SETTING_EXPRESSION: {
            _currentSelection = AUX_SETTING_AFTERTOUCH;
        } break;
    }
    display();
}

void AuxOutputMenu::onDownPressed() {
    switch (_currentSelection) {
        case AUX_SETTING_AFTERTOUCH: {
            _currentSelection = AUX_SETTING_EXPRESSION;
        } break;
        case AUX_SETTING_EXPRESSION: break;
    }
    display();
}
