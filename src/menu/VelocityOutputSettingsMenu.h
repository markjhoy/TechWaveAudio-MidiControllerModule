/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_VELOCITYOUTPUTSETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_VELOCITYOUTPUTSETTINGSMENU_H

#include <string>
#include "BaseMenu.h"
#include "RangeEditorMenu.h"
#include "../SettingsMenuSystem.h"

class VelocityOutputSettingsMenu : public BaseMenu {
public:
    VelocityOutputSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
        _velocityAdjustMenu = new RangeEditorMenu(
            _lcdDisplay,
            _menuSystem,
            _systemState,
            this,
            "Velocity Adjust",
             "  (raw value)",
            MIN_VELOCITY_ADJUST,
            MAX_VELOCITY_ADJUST,
            [this](float value) { _systemState->velocityAdjust = static_cast<int>(value); },
            1.0f
        );
    }

    ~VelocityOutputSettingsMenu() override {
        delete _velocityAdjustMenu;
    }

    void display() override {
        _lcdDisplay->clear();
        std::vector<std::string> choices = {
            "<< back",
            "velocity adjust",
            (_systemState->velocityCVMaxVoltage == TenVoltOutput ? "velocity +10v" : "velocity +5v")
        };
        _lcdDisplay->showMenu(getMenuName(), choices.data(), _currentMenuPosition, static_cast<int>(choices.size()));
    }

    std::string getMenuName() override { return "Velocity Setting"; }

protected:
    void menuInit() override {
        _customDisplay = true;
    }

    bool onMenuItemSelected(int menuItemIndex) override {
        // nothing to do as using custom display
        return false;
    }

    bool onBeforeMenuItemSelected(int menuItemIndex) override {
        switch (menuItemIndex) {
            case 0: {
                // back
                _menuSystem->changeMenu(_previousMenu);
                return false;
            }
            case 1: {
                // velocity adjust
                _velocityAdjustMenu->setCurrentValue(_systemState->velocityAdjust);
                _menuSystem->changeMenu(_velocityAdjustMenu);
                return false;
            }
            case 2: {
                // voltage select
                if (_systemState->velocityCVMaxVoltage == TenVoltOutput) {
                    _systemState->velocityCVMaxVoltage = FiveVoltOutput;
                } else {
                    _systemState->velocityCVMaxVoltage = TenVoltOutput;
                }
                display();
                return false;
            }
            default: {
                return true;
            }
        }
    }

    bool onBeforeLeftRotation(int currentMenuItemIndex) override {
        _currentMenuPosition--;
        if (_currentMenuPosition < 0) {
            _currentMenuPosition = 2;
        }
        display();
        return false;
    }

    bool onBeforeRightRotation(int currentMenuItemIndex) override {
        _currentMenuPosition++;
        if (_currentMenuPosition > 2) {
            _currentMenuPosition = 0;
        }
        display();
        return false;
    }

private:
    RangeEditorMenu *_velocityAdjustMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_VELOCITYOUTPUTSETTINGSMENU_H