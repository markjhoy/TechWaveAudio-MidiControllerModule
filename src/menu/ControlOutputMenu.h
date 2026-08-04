
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H

#include <string>
#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class ControlOutputMenu : public OutputCVMappingMenu {
public:
    ControlOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return " Control Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override;

    OutputMappingRoute getCurrentRouteMapping() override;

    void onRouteSettingChanged(OutputMappingRoute newRoute) override;
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_CONTROLOUTPUTMENU_H
