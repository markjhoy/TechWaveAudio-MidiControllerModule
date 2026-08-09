/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-05.
//

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_UTILITIESMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_UTILITIESMENU_H
#include "BaseMenu.h"
#include "CalibrationMenu.h"
#include "TuningMenu.h"
#include "../SettingsMenuSystem.h"


class UtilitiesMenu : public BaseMenu {
public:
    UtilitiesMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
        _tuningMenu = new TuningMenu(_lcdDisplay,_menuSystem,_systemState,this);
        _calibrationMenu = new CalibrationMenu(_lcdDisplay,_menuSystem,_systemState,this);
    }

    ~UtilitiesMenu() override {
        delete _tuningMenu;
        delete _calibrationMenu;
    }

    std::string getMenuName() override { return "   Utilities"; }

protected:
    void menuInit() override {
        std::vector<std::string> choices = {
            "tuning utilities",
            "calibration",
        };
        setMenuItems(choices);
    }

    bool onMenuItemSelected(int menuItemIndex) override {
        switch (menuItemIndex) {
            case 0: {
                _menuSystem->changeMenu(_tuningMenu);
                return false;
            }
            case 1: {
                _menuSystem->changeMenu(_calibrationMenu);
                return false;
            }
            default: {
                return true;
            }
        }
    }

private:
    TuningMenu *_tuningMenu = nullptr;
    CalibrationMenu *_calibrationMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_UTILITIESMENU_H