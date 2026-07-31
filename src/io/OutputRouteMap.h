
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#define TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H
#include <cstdint>
#include <vector>

#include "../TechWaveAudio_MidiControllerModule.h"
#include "pico/critical_section.h"

typedef struct OutputMappingRouteItem_t {
    OutputMappingRoute route = OutputMappingRoute_None;
    OutputMappingOutput output = OutputMappingOutput_None;
} OutputMappingRouteItem;

class OutputRouteMap {
public:
    OutputRouteMap();
    ~OutputRouteMap();

    void updateRoutes(const std::vector<OutputMappingRouteItem> &newRoutes);
    uint16_t getRouteMapping(OutputMappingRoute route);
    OutputMappingRoute getRouteForOutput(OutputMappingOutput output);

private:
    volatile uint16_t *_routes;
    int _mapLock_lockNum = 0;
    critical_section_t _mapLock;

    volatile OutputMappingRoute _currentAuxRoute = OutputMappingRoute_None;
    volatile OutputMappingRoute _currentCtlRoute = OutputMappingRoute_None;
};


#endif //TECHWAVEAUDIO_MCM_OUTPUTROUTEMAP_H