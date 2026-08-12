/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_V2_0_0_NOTEOUTPUTSETTINGSMENU_H
#define TECHWAVEAUDIO_MCM_V2_0_0_NOTEOUTPUTSETTINGSMENU_H
#include "BaseMenu.h"
#include "NotePriorityMenu.h"
#include "RangeEditorMenu.h"


class NoteOutputSettingsMenu : public BaseMenu {
public:
    NoteOutputSettingsMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState,
        BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
        _pitchAdjustMenu = new RangeEditorMenu(
            _lcdDisplay,
            _menuSystem,
            _systemState,
            this,
            "Pitch Adjust",
             "  (raw value)",
            MIN_PITCH_ADJUST,
            MAX_PITCH_ADJUST,
            [this](float value) { _systemState->pitchAdjust = value;; },
            1.0f
        );
        _notePriorityMenu = new NotePriorityMenu(_lcdDisplay, _menuSystem, _systemState, this);
        _pitchBendRangeMenu = new RangeEditorMenu(
            _lcdDisplay,
            _menuSystem,
            _systemState,
            this,
            "Pitch Bend Range",
            "  (in octaves)",
            0.0f,
            MAX_PITCH_BEND_RANGE_OCTAVES,
            [this](float value) { _systemState->pitchBendRange = value; },
            0.25f
        );
    }

    ~NoteOutputSettingsMenu() override {
        delete _pitchAdjustMenu;
        delete _notePriorityMenu;
        delete _pitchBendRangeMenu;
    }

    void display() override;

    std::string getMenuName() override { return "Note Output"; }

protected:
    void menuInit() override {
        _customDisplay = true;
    }

    bool onMenuItemSelected(int menuItemIndex) override {
        // never called
        return false;
    }

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

    bool onBeforeLeftRotation(int currentMenuItemIndex) override {
        _currentMenuPosition--;
        if (_currentMenuPosition < 0) {
            _currentMenuPosition = 4;
        }
        display();
        return false;
    }

    bool onBeforeRightRotation(int currentMenuItemIndex) override {
        _currentMenuPosition++;
        if (_currentMenuPosition > 4) {
            _currentMenuPosition = 0;
        }
        display();
        return false;
    }

private:
    RangeEditorMenu *_pitchAdjustMenu = nullptr;
    NotePriorityMenu  *_notePriorityMenu = nullptr;
    RangeEditorMenu *_pitchBendRangeMenu = nullptr;
};


#endif //TECHWAVEAUDIO_MCM_V2_0_0_NOTEOUTPUTSETTINGSMENU_H