
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H
#define TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H
#include "BaseMenu.h"


class OutputCVMappingMenu : public BaseMenu {
protected:
    OutputCVMappingMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    std::vector<OutputMappingRoute> _availableRoutes;
    int _currentRoutingIndex = 0;

    /**
     * Gets the list of available routings (in order) for this output
     * @return list of routes in menu order
     */
    virtual std::vector<OutputMappingRoute> getAvailableRoutes() = 0;

    virtual OutputMappingRoute getCurrentRouteMapping() = 0;

    /**
     * Called when the user selects a new route for this output
     * @param newRoute the new route setting
     */
    virtual void onRouteSettingChanged(OutputMappingRoute newRoute) = 0;

    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;
};


#endif //TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H