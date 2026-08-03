
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H
#define TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H

#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class ClockOutputMenu : public OutputCVMappingMenu {
public:
    ClockOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    inline std::string getMenuName() override { return "Clock Divisions"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return std::vector<OutputMappingRoute>{
            OutputMappingRoute_ClockTick,
            OutputMappingRoute_ClockTick_2,
            OutputMappingRoute_ClockTick_4,
            OutputMappingRoute_ClockTick_6,
            OutputMappingRoute_ClockTick_8,
            OutputMappingRoute_ClockTick_12,
            OutputMappingRoute_ClockTick_24,
        };
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->clockOutputMapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->clockOutputMapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};


#endif //TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H