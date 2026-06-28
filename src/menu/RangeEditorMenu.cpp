
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

void RangeEditorMenu::init() {
    _lcdDisplay->clear();
    _lcdDisplay->setTitle(_title);
}

void RangeEditorMenu::display() {
    float totalRange = std::fabs(_maxVal - _minVal);
    int currentValueIndex = std::floor(((_currentValue - _minVal) / totalRange) * RANGE_DISPLAY_STEPS);
    if (currentValueIndex >= RANGE_DISPLAY_STEPS) {
        currentValueIndex = RANGE_DISPLAY_STEPS - 1;
    }

    std::stringstream valueLine;
    if (_currentValue == std::floor(_currentValue)) {
        valueLine << "Value: " << static_cast<int>(_currentValue);
    } else {
        valueLine << "Value: " << std::fixed << std::setprecision(2) << _currentValue;
    }

    std::stringstream thisLine;
    thisLine << "[";
    for (int i = 0; i < RANGE_DISPLAY_STEPS; i++) {
        if (i == currentValueIndex) {
            thisLine << "|";
        } else {
            thisLine << ".";
        }
    }
    thisLine << "]";

    std::string clearLine = "                ";
    _lcdDisplay->writeLineAt(1, clearLine);
    _lcdDisplay->writeLineAt(2, clearLine);
    _lcdDisplay->writeLineAt(3, clearLine);

    _lcdDisplay->writeLineAt(1, valueLine.str());
    _lcdDisplay->writeLineAt(2, thisLine.str());
    _lcdDisplay->writeLineAt(3, _unitsDisplay);
    _lcdDisplay->show();
}

void RangeEditorMenu::onEnterPressed() {
    if (_onChangeCallback) {
        _onChangeCallback(_currentValue);
    }
    _menuSystem->changeMenu(_previousMenu);
}

void RangeEditorMenu::onBackPressed() {
    _menuSystem->changeMenu(_previousMenu);
}

void RangeEditorMenu::onNextPressed() {
    // nothing to do
}

void RangeEditorMenu::onUpPressed() {
    _currentValue += _step;
    if (_currentValue > _maxVal) {
        _currentValue = _maxVal;
    }
    display();
}

void RangeEditorMenu::onDownPressed() {
    _currentValue -= _step;
    if (_currentValue < _minVal) {
        _currentValue = _minVal;
    }
    display();
}
