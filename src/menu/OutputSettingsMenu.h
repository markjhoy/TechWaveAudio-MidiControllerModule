/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

//
// Created by Mark J. Hoy on 2026-08-05.
//

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTSETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTSETTINGSMENU_H
#include "BaseMenu.h"
#include "ClockOutputMenu.h"
#include "NoteOutputSettingsMenu.h"
#include "Out1OutputMenu.h"
#include "Out2OutputMenu.h"
#include "OutX1OutputMenu.h"
#include "OutX2OutputMenu.h"
#include "OutX3OutputMenu.h"
#include "OutX4OutputMenu.h"
#include "SelectableOutputSettingsMenu.h"
#include "TriggerDurationMenu.h"
#include "VelocityOutputSettingsMenu.h"
#include "../SettingsMenuSystem.h"


class OutputSettingsMenu : public BaseMenu {
public:
    OutputSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
                       BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
        _noteOutputSettingsMenu = new NoteOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this);
        _velocityOutputSettingsMenu = new VelocityOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this);

        _out1SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new Out1OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->out1CVMaxVoltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->out1CVMaxVoltage = newValue; },
            "Out 1"
        );
        _out2SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new Out2OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->out2CVMaxVoltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->out2CVMaxVoltage = newValue; },
            "Out 2"
        );
        _outX1SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new OutX1OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->outX1Voltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->outX1Voltage = newValue; },
            "Out X1"
        );
        _outX2SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new OutX2OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->outX2Voltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->outX2Voltage = newValue; },
            "Out X2"
        );
        _outX3SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new OutX3OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->outX3Voltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->outX3Voltage = newValue; },
            "Out X3"
        );
        _outX4SettingsMenu = new SelectableOutputSettingsMenu(_lcdDisplay, _menuSystem, _systemState, this,
            [this](BaseMenu *parent) { return new OutX4OutputMenu(_lcdDisplay, _menuSystem, _systemState, parent); },
            [this]() { return this->_systemState->outX4Voltage; },
            [this](CVOutputVoltsValue newValue) { this->_systemState->outX4Voltage = newValue; },
            "Out X4"
        );

        _triggerDurationMenu = new TriggerDurationMenu(_lcdDisplay, _menuSystem, _systemState, this);
        _clockOutputMenu = new ClockOutputMenu(_lcdDisplay, _menuSystem, _systemState, this);
    }

    ~OutputSettingsMenu() override {
        delete _noteOutputSettingsMenu;
        delete _velocityOutputSettingsMenu;
        delete _out1SettingsMenu;
        delete _out2SettingsMenu;
        delete _outX1SettingsMenu;
        delete _outX2SettingsMenu;
        delete _outX3SettingsMenu;
        delete _outX4SettingsMenu;
        delete _clockOutputMenu;
        delete _noteOutputSettingsMenu;
    }

    std::string getMenuName() override { return "Output Settings"; }

protected:
    void menuInit() override;
    bool onMenuItemSelected(int menuItemIndex) override;

private:
    NoteOutputSettingsMenu *_noteOutputSettingsMenu = nullptr;
    VelocityOutputSettingsMenu *_velocityOutputSettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_out1SettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_out2SettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_outX1SettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_outX2SettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_outX3SettingsMenu = nullptr;
    SelectableOutputSettingsMenu *_outX4SettingsMenu = nullptr;
    TriggerDurationMenu *_triggerDurationMenu = nullptr;
    ClockOutputMenu *_clockOutputMenu = nullptr;

    Out1OutputMenu *_out1OutputMenu = nullptr;
    Out2OutputMenu *_out2OutputMenu = nullptr;
    OutX1OutputMenu *_outX1OutputMenu = nullptr;
    OutX2OutputMenu *_outX2OutputMenu = nullptr;
    OutX3OutputMenu *_outX3OutputMenu = nullptr;
    OutX4OutputMenu *_outX4OutputMenu = nullptr;

    void setupMenus();
    bool onExpandedMenuItemSelected(int menuItemIndex);
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_OUTPUTSETTINGSMENU_H