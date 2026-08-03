
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "OutputRouteMap.h"

OutputRouteMap::OutputRouteMap() {
    sem_init(&_lockRouteMapping, 1, 1);

    _routes = new uint16_t[OutputMappingRoute_MAX_ROUTES];
    for (int i = 0; i < OutputMappingRoute_MAX_ROUTES; i++) {
        _routes[i] = 0;
    }
}

OutputRouteMap::~OutputRouteMap() {
    sem_reset(&_lockRouteMapping, 1);
    delete[] _routes;
}

void OutputRouteMap::updateRoutes(const std::vector<OutputMappingRouteItem> &newRoutes) {
    auto *newRouteMap = new uint16_t[OutputMappingRoute_MAX_ROUTES];
    for (int i = 0; i < OutputMappingRoute_MAX_ROUTES; i++) {
        newRouteMap[i] = 0;
    }

    for (auto route : newRoutes) {
        newRouteMap[route.route] |= route.output;
        switch (route.output) {
            case OutputMappingOutput_Aux:
                { _currentAuxRoute = route.route; } break;
            case OutputMappingOutput_Control:
                { _currentCtlRoute = route.route; } break;
            default:
                break;
        }
    }

    uint32_t status = save_and_disable_interrupts();
    sem_acquire_blocking(&_lockRouteMapping);
    delete[] _routes;
    _routes = newRouteMap;
    sem_release(&_lockRouteMapping);
    restore_interrupts(status);
}

uint16_t OutputRouteMap::getRouteMapping(OutputMappingRoute route) {
    uint32_t status = save_and_disable_interrupts();
    sem_acquire_blocking(&_lockRouteMapping);
    uint16_t ret = _routes[route];
    sem_release(&_lockRouteMapping);
    restore_interrupts(status);
    return ret;
}

OutputMappingRoute OutputRouteMap::getRouteForOutput(OutputMappingOutput output) {
    switch (output) {
        case OutputMappingOutput_Aux:
            return _currentAuxRoute;;
        case OutputMappingOutput_Control:
            return _currentCtlRoute;;
        default:
            return OutputMappingRoute_None;
    }
}
