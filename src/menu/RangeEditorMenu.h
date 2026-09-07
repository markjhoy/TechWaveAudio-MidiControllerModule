
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RANGEEDITORMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RANGEEDITORMENU_H
#include <functional>
#include <string>

#include "BaseMenu.h"
#include "../TechWaveAudio_MidiControllerModule.h"

/**
 * A range editor. Used for various settings.
 */
class RangeEditorMenu : public BaseMenu {
public:
    RangeEditorMenu(
        OledDisplay *lcdDisplay,
        IMenuSystemHandler *menuSystem,
        SystemState *systemState,
        BaseMenu *previousMenu,
        const std::string &title,
        const std::string &unitsDisplay,
        float minVal,
        float maxVal,
        const RangeEditorCallback &onChangeCallback,
        float step = 1.0f
    );
    ~RangeEditorMenu() override;

    void setCurrentValue(float value) { _currentValue = value; }

    void setOnValueEditedCallback(const RangeEditorCallback &callback) { _onValueEditedCallback = callback; }

    void display() override;

    void onMenuChanging() override;

    std::string getMenuName() override { return _title; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override { /* nothing to do */ return false; }

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

    bool onBeforeLeftRotation(int currentMenuItemIndex) override;

    bool onBeforeRightRotation(int currentMenuItemIndex) override;

    bool onBackPressed() override;

private:
    std::string _title;
    std::string _unitsDisplay;
    float _minVal = 0;
    float _maxVal = 0;
    float _currentValue = 0;
    float _step = 1.0f;
    RangeEditorCallback _onChangeCallback = nullptr;
    RangeEditorCallback _onValueEditedCallback = nullptr;
    bool _isEditing = false;
    bool _editSelected = false;
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RANGEEDITORMENU_H