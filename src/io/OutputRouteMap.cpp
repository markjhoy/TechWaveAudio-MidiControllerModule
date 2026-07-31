//
// Created by Mark J. Hoy on 2026-07-28.
//

#include "OutputRouteMap.h"

OutputRouteMap::OutputRouteMap() {
    critical_section_init(&_mapLock);

    _routes = new uint16_t[OutputMappingRoute_MAX_ROUTES];
    for (int i = 0; i < OutputMappingRoute_MAX_ROUTES; i++) {
        _routes[i] = 0;
    }
}

OutputRouteMap::~OutputRouteMap() {
    critical_section_deinit(&_mapLock);
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

    critical_section_enter_blocking(&_mapLock);
    delete[] _routes;
    _routes = newRouteMap;
    critical_section_exit(&_mapLock);
}

uint16_t OutputRouteMap::getRouteMapping(OutputMappingRoute route) {
    critical_section_enter_blocking(&_mapLock);
    uint16_t ret = _routes[route];
    critical_section_exit(&_mapLock);
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
