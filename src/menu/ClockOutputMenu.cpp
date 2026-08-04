
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "ClockOutputMenu.h"

std::vector<OutputMappingRoute> ClockOutputMenu::getAvailableRoutes() {
    return std::vector<OutputMappingRoute>{
        OutputMappingRoute_ClockTick,
        OutputMappingRoute_ClockTick_2,
        OutputMappingRoute_ClockTick_4,
        OutputMappingRoute_ClockTick_6,
        OutputMappingRoute_ClockTick_8,
        OutputMappingRoute_ClockTick_12,
        OutputMappingRoute_ClockTick_24,
    };
}

OutputMappingRoute ClockOutputMenu::getCurrentRouteMapping() {
    return _systemState->clockOutputMapping;
}

void ClockOutputMenu::onRouteSettingChanged(OutputMappingRoute newRoute) {
    _systemState->clockOutputMapping = newRoute;
    global_core0_handler->sendRouteMappingUpdateSignal();
}