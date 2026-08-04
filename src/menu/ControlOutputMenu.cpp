
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "ControlOutputMenu.h"

std::vector<OutputMappingRoute> ControlOutputMenu::getAvailableRoutes() {
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

OutputMappingRoute ControlOutputMenu::getCurrentRouteMapping() {
    return _systemState->ctlOutMapping;
}

void ControlOutputMenu::onRouteSettingChanged(OutputMappingRoute newRoute) {
    _systemState->ctlOutMapping = newRoute;
    global_core0_handler->sendRouteMappingUpdateSignal();
}