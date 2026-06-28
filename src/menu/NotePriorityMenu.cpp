
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "NotePriorityMenu.h"

#include "../SettingsMenuSystem.h"

void NotePriorityMenu::display() {
    _lcdDisplay->clear();
    _lcdDisplay->setTitle("Note Priority");
    if (_systemState->notePriority == NOTE_PRIORITY_LAST) {
        _lcdDisplay->writeLineAt(1, "* Last Note", _currentSelection == NOTE_PRIORITY_LAST);
    } else {
        _lcdDisplay->writeLineAt(1, "  Last Note", _currentSelection == NOTE_PRIORITY_LAST);
    }
    if (_systemState->notePriority == NOTE_PRIORITY_HIGHEST) {
        _lcdDisplay->writeLineAt(2, "* Highest Note", _currentSelection == NOTE_PRIORITY_HIGHEST);
    } else {
        _lcdDisplay->writeLineAt(2, "  Highest Note", _currentSelection == NOTE_PRIORITY_HIGHEST);
    }
    if (_systemState->notePriority == NOTE_PRIORITY_LOWEST) {
        _lcdDisplay->writeLineAt(3, "* Lowest Note", _currentSelection == NOTE_PRIORITY_LOWEST);
    } else {
        _lcdDisplay->writeLineAt(3, "  Lowest Note", _currentSelection == NOTE_PRIORITY_LOWEST);
    }
    _lcdDisplay->show();
}

void NotePriorityMenu::onEnterPressed() {
    _systemState->notePriority = _currentSelection;
    display();
}

void NotePriorityMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void NotePriorityMenu::onUpPressed() {
    switch (_currentSelection) {
        case NOTE_PRIORITY_LAST: break;
        case NOTE_PRIORITY_HIGHEST: {
            _currentSelection = NOTE_PRIORITY_LAST;
        } break;
        case NOTE_PRIORITY_LOWEST: {
            _currentSelection = NOTE_PRIORITY_HIGHEST;
        }
    }
    display();
}

void NotePriorityMenu::onDownPressed() {
    switch (_currentSelection) {
        case NOTE_PRIORITY_LAST: {
            _currentSelection = NOTE_PRIORITY_HIGHEST;
        } break;
        case NOTE_PRIORITY_HIGHEST: {
            _currentSelection = NOTE_PRIORITY_LOWEST;
        } break;
        case NOTE_PRIORITY_LOWEST: break;
    }
    display();
}
