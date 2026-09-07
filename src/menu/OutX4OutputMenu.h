/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX4OUTPUTMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX4OUTPUTMENU_H

#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

/**
 * Menu to select routing for the X4 CV output
 */
class OutX4OutputMenu : public OutputCVMappingMenu {
public:
    OutX4OutputMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "  OutX4 Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return standard_assignable_routes;
    }

    OutputMappingRoute getCurrentRouteMapping() override {
        return _systemState->outX4Mapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->outX4Mapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX4OUTPUTMENU_H
