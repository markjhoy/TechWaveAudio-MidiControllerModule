//
// Created by Mark J. Hoy on 2026-07-28.
//

#ifndef TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#define TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#include <cstdint>
#include <vector>

#include "TechWaveAudio_MidiControllerModule.h"
#include "hardware/sync/spin_lock.h"

typedef struct OutputMappingRouteItem_t {
    OutputMappingRoute route = OutputMappingRoute_None;
    OutputMappingOutput output = OutputMappingOutput_None;
} OutputMappingRouteItem;

class OutputRouteMap {
public:
    OutputRouteMap() {
        _mapLock_lockNum = spin_lock_claim_unused(true);
        _mapLock_spinlock = spin_lock_init(_mapLock_lockNum);

        _routes = new uint16_t[OutputMappingRoute_MAX_ROUTINGS];
        for (int i = 0; i < OutputMappingRoute_MAX_ROUTINGS; i++) {
            _routes[i] = 0;
        }
    }

    ~OutputRouteMap() {
        spin_lock_unclaim(_mapLock_lockNum);
        delete[] _routes;
    }

    void updateRoutes(const std::vector<OutputMappingRouteItem> &newRoutes) {
        auto *newRouteMap = new uint16_t[OutputMappingRoute_MAX_ROUTINGS];
        for (int i = 0; i < OutputMappingRoute_MAX_ROUTINGS; i++) {
            newRouteMap[i] = 0;
        }

        for (auto route : newRoutes) {
            newRouteMap[route.route] |= route.output;
        }

        auto interrupts = spin_lock_blocking(_mapLock_spinlock);
        delete[] _routes;
        _routes = newRouteMap;
        spin_unlock(_mapLock_spinlock, interrupts);
    }

    uint16_t getRouteMapping(OutputMappingRoute route) {
        auto interrupts = spin_lock_blocking(_mapLock_spinlock);
        uint16_t ret = _routes[route];
        spin_unlock(_mapLock_spinlock, interrupts);
        return ret;
    }

    OutputMappingRoute getRouteForOutput(OutputMappingOutput output) {
        OutputMappingRoute outRoute = OutputMappingRoute_None;

        auto interrupts = spin_lock_blocking(_mapLock_spinlock);
        for (int i = 0; i < OutputMappingRoute_MAX_ROUTINGS; i++) {
            if ((_routes[i] & output) > 0) {
                outRoute = static_cast<OutputMappingRoute>(i);
                break;
            }
        }
        spin_unlock(_mapLock_spinlock, interrupts);

        return outRoute;
    }
private:
    volatile uint16_t *_routes;
    int _mapLock_lockNum = 0;
    spin_lock_t *_mapLock_spinlock;
};


#endif //TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H