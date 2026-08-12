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

class OutX4OutputMenu : public OutputCVMappingMenu {
public:
    OutX4OutputMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::string getMenuName() override { return "  OutX4 Output"; }

protected:
    std::vector<OutputMappingRoute> getAvailableRoutes() override {
        return std::vector<OutputMappingRoute>{
            OutputMappingRoute_None,
            OutputMappingRoute_ModWheel,
            OutputMappingRoute_Aftertouch,
            OutputMappingRoute_Expression,
            OutputMappingRoute_Effect_1,
            OutputMappingRoute_Effect_2,
            OutputMappingRoute_Gate,
            OutputMappingRoute_Trigger,
            OutputMappingRoute_Run,
            OutputMappingRoute_Reset,
            OutputMappingRoute_Note,
            OutputMappingRoute_Velocity,
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
        return _systemState->outX4Mapping;
    }

    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->outX4Mapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }
};

#endif //TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_OUTX4OUTPUTMENU_H
