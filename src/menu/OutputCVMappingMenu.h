
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
public:

    void init() override;

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override { /* nothing to do */ }

    void onUpPressed() override;

    void onDownPressed() override;

protected:
    OutputCVMappingMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    int _currentMenuIndex = 0;
    int _setRouteMenuIndex = 0;
    std::vector<OutputMappingRoute> _availableRoutes;
    std::vector<std::string> _availableRoutesNames;

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
};


#endif //TECHWAVEAUDIO_MCM_OUTPUTCVMAPPINGMENU_H