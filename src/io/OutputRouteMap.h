
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#define TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#include <map>
#include <set>
#include <vector>

#include "../TechWaveAudio_MidiControllerModule.h"
#include "pico/sem.h"

typedef struct OutputMappingRouteItem_t {
    OutputMappingRoute route = OutputMappingRoute_None;
    OutputMappingOutput output = OutputMappingOutput_None;
} OutputMappingRouteItem;

/**
 * A mapping of routes to outputs
 */
class OutputRouteMap {
public:
    OutputRouteMap();
    ~OutputRouteMap();

    /**
     * Updates the entire set of routes
     * @param newRoutes the new routeings
     */
    void updateRoutes(const std::vector<OutputMappingRouteItem> &newRoutes);

    /**
     * Gets the output bitmap for a route
     * @param route the route
     * @return the bitmap of OutputMappingOutput routes
     */
    uint16_t getRouteMapping(OutputMappingRoute route);

    /**
     * Gets the routing for a single output
     * @param output The output
     * @return the route for the output
     */
    OutputMappingRoute getRouteForOutput(OutputMappingOutput output);

    /**
     * Gets the routes that are in use for routing any clock events
     * to outputs.
     * @return The set of routes that are in use for clock events
     */
    std::set<OutputMappingRoute> getSetClockRoutes() { return _currentClockRoutes; }

private:
    uint16_t *_routes;
    semaphore_t _lockRouteMapping{};
    std::set<OutputMappingRoute> _currentClockRoutes;
    std::map<OutputMappingOutput, OutputMappingRoute> _currentRoutes;
};


#endif //TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H