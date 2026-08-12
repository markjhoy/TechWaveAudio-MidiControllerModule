/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_SELECTABLEOUTPUTSETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_SELECTABLEOUTPUTSETTINGSMENU_H
#include <sstream>

#include "BaseMenu.h"
#include "OutputCVMappingMenu.h"
#include "../SettingsMenuSystem.h"

class SelectableOutputSettingsMenu : public BaseMenu {
public:
    SelectableOutputSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
                                 BaseMenu *previousMenu,
                                 const std::function<OutputCVMappingMenu*(BaseMenu *)> &createRouteMappingMenu,
                                 const std::function<CVOutputVoltsValue(void)> &getCurrentVoltageOutput,
                                 const std::function<void(CVOutputVoltsValue)> &onOutputVoltageChanged,
                                 const std::string &outputName)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {

        this->_routeMappingMenu = createRouteMappingMenu(this);
        _getCurrentVoltageOutput = getCurrentVoltageOutput;
        _onOutputVoltageChanged = onOutputVoltageChanged;

        std::stringstream titleStr;
        titleStr << outputName << " Settings";
        _outputName = outputName;
        _menuTitle = titleStr.str();
    }

    ~SelectableOutputSettingsMenu() override {
        delete _routeMappingMenu;
    }

    void display() override;

    std::string getMenuName() override { return _menuTitle; }

protected:
    void menuInit() override {
        _customDisplay = true;
    }

    bool onMenuItemSelected(int menuItemIndex) override {
        // never called - custom display
        return false;
    }

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

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
    std::function<OutputCVMappingMenu*(void)> _createRouteMappingMenu;
    std::function<CVOutputVoltsValue()> _getCurrentVoltageOutput;
    std::function<void(CVOutputVoltsValue)> _onOutputVoltageChanged;
    std::string _menuTitle;
    std::string _outputName;

    OutputCVMappingMenu *_routeMappingMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_SELECTABLEOUTPUTSETTINGSMENU_H