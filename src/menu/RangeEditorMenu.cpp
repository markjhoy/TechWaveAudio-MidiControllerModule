
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "RangeEditorMenu.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "../SettingsMenuSystem.h"

RangeEditorMenu::RangeEditorMenu(
    OledDisplay *lcdDisplay,
    SettingsMenuSystem *menuSystem,
    SystemState *systemState,
    BaseMenu *previousMenu,
    const std::string &title,
    const std::string &unitsDisplay,
    float minVal,
    float maxVal,
    const RangeEditorCallback &onChangeCallback,
    float step
 ) : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    _title = title;
    _unitsDisplay = unitsDisplay;
    _minVal = minVal;
    _maxVal = maxVal;
    _onChangeCallback = onChangeCallback;
    _step = step;
}

RangeEditorMenu::~RangeEditorMenu() = default;

void RangeEditorMenu::menuInit() {
    _customDisplay = true;
    _isEditing = false;
    _editSelected = false;
    _lcdDisplay->clear();
    _lcdDisplay->setTitle(_title);
}

void RangeEditorMenu::onMenuChanging() {
    BaseMenu::onMenuChanging();
}

bool RangeEditorMenu::onBeforeMenuItemSelected(int menuItemIndex) {
    // enter was pressed
    if (_isEditing) {
        _isEditing = false;
        _editSelected = false;
    } else {
        if (_editSelected) {
            // edit was pressed
            _isEditing = true;
        }
        else {
            // back was pressed
            if (_onChangeCallback) {
                _onChangeCallback(_currentValue);
            }
            _menuSystem->changeMenu(_previousMenu);
        }
    }
    display();
    return false;
}

bool RangeEditorMenu::onBeforeLeftRotation(int currentMenuItemIndex) {
    if (_isEditing) {
        // change the value
        _currentValue -= _step;
        if (_currentValue < _minVal) {
            _currentValue = _minVal;
        }
    } else {
        // change the back / edit function
        _editSelected = !_editSelected;
    }
    display();
    return false;
}

bool RangeEditorMenu::onBeforeRightRotation(int currentMenuItemIndex) {
    if (_isEditing) {
        // change the value
        _currentValue += _step;
        if (_currentValue > _maxVal) {
            _currentValue = _maxVal;
        }
    } else {
        // change the back / edit function
        _editSelected = !_editSelected;
    }
    display();
    return false;
}

bool RangeEditorMenu::onBackPressed() {
    return BaseMenu::onBackPressed();
}

void RangeEditorMenu::display() {
    float totalRange = std::fabs(_maxVal - _minVal);
    int currentValueIndex = std::floor(((_currentValue - _minVal) / totalRange) * RANGE_DISPLAY_STEPS);
    if (currentValueIndex >= RANGE_DISPLAY_STEPS) {
        currentValueIndex = RANGE_DISPLAY_STEPS - 1;
    }

    _lcdDisplay->clearArea(0, 16, 128, 48);

    std::stringstream valueLine;
    if (_currentValue == std::floor(_currentValue)) {
        valueLine << "Value: " << static_cast<int>(_currentValue);
    } else {
        valueLine << "Value: " << std::fixed << std::setprecision(2) << _currentValue;
    }
    _lcdDisplay->writeLineAt(1, valueLine.str());

    if (_isEditing) {
        std::stringstream displayLine;
        displayLine << "[";
        for (int i = 0; i < RANGE_DISPLAY_STEPS; i++) {
            if (i == currentValueIndex) {
                displayLine << "|";
            } else {
                displayLine << ".";
            }
        }
        displayLine << "]";
        _lcdDisplay->writeLineAt(2, displayLine.str());
    } else {
        _lcdDisplay->writeTextAt(0, 32, "[ back ]", OledFontType_8x16, !_editSelected);
        _lcdDisplay->writeTextAt(64, 32, "[ edit ]", OledFontType_8x16, _editSelected);
    }

    _lcdDisplay->writeLineAt(3, _unitsDisplay);
    _lcdDisplay->show();
}
