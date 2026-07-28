
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
#include "BaseMenu.h"
#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class ControlOutputMenu : public OutputCVMappingMenu {
public:
    ControlOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "Control Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return std::vector<OutputMappingRoute>{
            OutputMappingRoute_ModWheel,
            OutputMappingRoute_Aftertouch,
            OutputMappingRoute_Expression,
            OutputMappingRoute_Effect_1,
            OutputMappingRoute_Effect_2,
        };
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->ctlOutMapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->ctlOutMapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};


#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H