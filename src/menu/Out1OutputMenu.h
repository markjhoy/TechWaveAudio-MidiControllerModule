
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT1CONTROLMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT1CONTROLMENU_H

#include <string>
#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

/**
 * Routing menu for out 1
 */
class Out1OutputMenu : public OutputCVMappingMenu {
public:
    Out1OutputMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    inline std::string getMenuName() override { return "  Out1 Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return standard_assignable_routes;
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->out1Mapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->out1Mapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT1CONTROLMENU_H