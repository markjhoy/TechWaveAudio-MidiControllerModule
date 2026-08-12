/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTROUTESETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTROUTESETTINGSMENU_H
#include "BaseMenu.h"
#include "Out1OutputMenu.h"
#include "Out2OutputMenu.h"
#include "OutX1OutputMenu.h"
#include "OutX2OutputMenu.h"
#include "OutX3OutputMenu.h"
#include "OutX4OutputMenu.h"


class OutputRouteSettingsMenu : public BaseMenu {
public:
    OutputRouteSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
        _out1OutputMenu = new Out1OutputMenu(lcdDisplay, menuSystem, systemState, this);
        _out2OutputMenu = new Out2OutputMenu(lcdDisplay, menuSystem, systemState, this);
        _outX1OutputMenu = new OutX1OutputMenu(lcdDisplay, menuSystem, systemState, this);
        _outX2OutputMenu = new OutX2OutputMenu(lcdDisplay, menuSystem, systemState, this);
        _outX3OutputMenu = new OutX3OutputMenu(lcdDisplay, menuSystem, systemState, this);
        _outX4OutputMenu = new OutX4OutputMenu(lcdDisplay, menuSystem, systemState, this);
    }

    ~OutputRouteSettingsMenu() override {
        delete _out1OutputMenu;
        delete _out2OutputMenu;
        delete _outX1OutputMenu;
        delete _outX2OutputMenu;
        delete _outX3OutputMenu;
        delete _outX4OutputMenu;
    }

    std::string getMenuName() override { return " Output Routing"; }

protected:
    void menuInit() override {
        std::vector<std::string> choices = {
            "Out 1",
            "Out 2"
        };

        if (_systemState->expansionSensed) {
            choices.push_back("Out X1");
            choices.push_back("Out X2");
            choices.push_back("Out X3");
            choices.push_back("Out X41");
        }
        this->setMenuItems(choices);
    }

    bool onMenuItemSelected(int menuItemIndex) override {
        switch (menuItemIndex) {
            case 0: {
                _menuSystem->changeMenu(_out1OutputMenu);
                return false;
            }
            case 1: {
                _menuSystem->changeMenu(_out2OutputMenu);
                return false;
            }
            case 2: {
                if (_systemState->expansionSensed) {
                    _menuSystem->changeMenu(_outX1OutputMenu);
                    return false;
                }
            }
            case 3: {
                if (_systemState->expansionSensed) {
                    _menuSystem->changeMenu(_outX2OutputMenu);
                    return false;
                }
            }
            case 4: {
                if (_systemState->expansionSensed) {
                    _menuSystem->changeMenu(_outX3OutputMenu);
                    return false;
                }
            }
            case 5: {
                if (_systemState->expansionSensed) {
                    _menuSystem->changeMenu(_outX4OutputMenu);
                    return false;
                }
            }
        }

        return true;
    }

private:
    Out1OutputMenu *_out1OutputMenu = nullptr;
    Out2OutputMenu *_out2OutputMenu = nullptr;

    OutX1OutputMenu *_outX1OutputMenu = nullptr;
    OutX2OutputMenu *_outX2OutputMenu = nullptr;
    OutX3OutputMenu *_outX3OutputMenu = nullptr;
    OutX4OutputMenu *_outX4OutputMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTROUTESETTINGSMENU_H