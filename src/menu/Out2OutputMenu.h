
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT2OUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT2OUTPUTMENU_H

#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

/**
 * Routing menu for out 2
 */
class Out2OutputMenu : public OutputCVMappingMenu {
public:
    Out2OutputMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "  Out2 Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return standard_assignable_routes;
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->out2Mapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->out2Mapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUT2OUTPUTMENU_H
