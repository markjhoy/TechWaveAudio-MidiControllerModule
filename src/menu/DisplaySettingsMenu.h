
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H
#define MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H
#include "BaseMenu.h"
#include "DisplayClockLedMenu.h"
#include "DisplayRefreshMenu.h"
#include "RangeEditorMenu.h"

/**
 * Branching menu for controlling the display settings
 */
class DisplaySettingsMenu : public BaseMenu {
public:
    DisplaySettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {

        _refreshMenu = new DisplayRefreshMenu(lcdDisplay, menuSystem, systemState, this);
        _clockLedMenu = new DisplayClockLedMenu(lcdDisplay, menuSystem, systemState, this);
        _brightnessMenu = new RangeEditorMenu(
            _lcdDisplay,
            _menuSystem,
            _systemState,
            this,
            "oled brightness",
             "  (intensity)",
            1,
            16,
            [this](const float value) {
                _systemState->screenBrightness = static_cast<uint8_t>(value);
                _lcdDisplay->setBrightness(_systemState->screenBrightness);
            },
            1.0f
        );
    }

    ~DisplaySettingsMenu() override {
        delete _refreshMenu;
        delete _clockLedMenu;
        delete _brightnessMenu;
    };

    std::string getMenuName() override { return "Display Settings"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

private:
    DisplayRefreshMenu *_refreshMenu = nullptr;
    DisplayClockLedMenu *_clockLedMenu = nullptr;
    RangeEditorMenu *_brightnessMenu = nullptr;

    void resetMenuChoices();
};


#endif //MIDI_CONTROLLER_MODULE_DISPLAYSETTINGSMENU_H