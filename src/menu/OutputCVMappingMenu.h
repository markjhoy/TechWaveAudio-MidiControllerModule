
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H
#define TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H
#include "BaseMenu.h"

/**
 * Base menu for routing options
 */
class OutputCVMappingMenu : public BaseMenu {
protected:
    OutputCVMappingMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    /**
     * Gets the list of available routings (in order) for this output
     * @return list of routes in menu order
     */
    virtual std::vector<OutputMappingRoute> getAvailableRoutes() = 0;

    /**
     * Gets the current routing of the output
     * @return the current routing for the output
     */
    virtual OutputMappingRoute getCurrentRouteMapping() = 0;

    /**
     * Called when the user selects a new route for this output
     * @param newRoute the new route setting
     */
    virtual void onRouteSettingChanged(OutputMappingRoute newRoute) = 0;

    /**
     * Initializes the menu with the routing options
     */
    void menuInit() override;

    /**
     * Called when a routing is selected by the user
     * @param menuItemIndex the index selected
     * @return true to refresh the display
     */
    bool onMenuItemSelected(int menuItemIndex) override;
private:
    std::vector<OutputMappingRoute> _availableRoutes;
    int _currentRoutingIndex = 0;

};


#endif //TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H