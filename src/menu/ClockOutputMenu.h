
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H
#define TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H

#include <string>
#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class ClockOutputMenu : public OutputCVMappingMenu {
public:
    ClockOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "Clock Divisions"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override;
    OutputMappingRoute getCurrentRouteMapping() override;
    void onRouteSettingChanged(OutputMappingRoute newRoute) override;
};

#endif //TECHWAVEAUDIO_MCM_CLOCKOUTPUTMENU_H
