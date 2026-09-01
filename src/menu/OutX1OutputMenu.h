
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX1OUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX1OUTPUTMENU_H

#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

/**
 * Menu to select routing for the X1 CV output
 */
class OutX1OutputMenu : public OutputCVMappingMenu {
public:
    OutX1OutputMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "  OutX1 Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return standard_assignable_routes;
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->outX1Mapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->outX1Mapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX1OUTPUTMENU_H
