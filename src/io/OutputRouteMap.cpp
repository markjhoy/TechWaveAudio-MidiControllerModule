
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
    _currentRoutes.clear();
    _currentClockRoutes.clear();
}

OutputRouteMap::~OutputRouteMap() {
    sem_reset(&_lockRouteMapping, 1);
    _currentRoutes.clear();
    _currentClockRoutes.clear();
    delete[] _routes;
}

void OutputRouteMap::updateRoutes(const std::vector<OutputMappingRouteItem> &newRoutes) {
    auto *newRouteMap = new uint16_t[OutputMappingRoute_MAX_ROUTES];
    for (int i = 0; i < OutputMappingRoute_MAX_ROUTES; i++) {
        newRouteMap[i] = 0;
    }

    const uint32_t status = save_and_disable_interrupts();
    sem_acquire_blocking(&_lockRouteMapping);

    _currentRoutes.clear();
    _currentClockRoutes.clear();

    for (auto [route, output] : newRoutes) {
        newRouteMap[route] |= output;
        _currentRoutes[output] = route;
        if (route >= ROUTE_CLOCK_TICK_START_VALUE)
            _currentClockRoutes.insert(route);
    }

    delete[] _routes;
    _routes = newRouteMap;
    sem_release(&_lockRouteMapping);
    restore_interrupts(status);
}

uint16_t OutputRouteMap::getRouteMapping(OutputMappingRoute route) {
    const uint32_t status = save_and_disable_interrupts();
    sem_acquire_blocking(&_lockRouteMapping);

    const uint16_t ret = _routes[route];

    sem_release(&_lockRouteMapping);
    restore_interrupts(status);

    return ret;
}

OutputMappingRoute OutputRouteMap::getRouteForOutput(OutputMappingOutput output) {
    uint32_t status = save_and_disable_interrupts();
    sem_acquire_blocking(&_lockRouteMapping);

    OutputMappingRoute ret = OutputMappingRoute_None;
    if (const auto it = _currentRoutes.find(output); it != _currentRoutes.end()) {
        ret = it->second;
    }

    sem_release(&_lockRouteMapping);
    restore_interrupts(status);

    return ret;
}
