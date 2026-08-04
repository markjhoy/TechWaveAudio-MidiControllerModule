
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H

#include <string>
#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class AuxOutputMenu : public OutputCVMappingMenu {
public:
    AuxOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    inline std::string getMenuName() override { return "   Aux Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override;

    OutputMappingRoute getCurrentRouteMapping() override;

    void onRouteSettingChanged(OutputMappingRoute newRoute) override;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H