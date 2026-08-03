
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H

#include "OutputCVMappingMenu.h"
#include "../GlobalHandlers.h"

class AuxOutputMenu : public OutputCVMappingMenu {
public:
    AuxOutputMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : OutputCVMappingMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    inline std::string getMenuName() override { return "Aux Output"; }
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
        return _systemState->auxOutMapping;
    }

    /**
     * Called when the user selects a new route for this output
     * @param newRoute the new route setting
     */
    void onRouteSettingChanged(OutputMappingRoute newRoute) override {
        _systemState->auxOutMapping = newRoute;
        global_core0_handler->sendRouteMappingUpdateSignal();
    }

};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_AUXCONTROLMENU_H