
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


class RangeEditorMenu : public BaseMenu {
public:
    RangeEditorMenu(
        OledDisplay *lcdDisplay,
        SettingsMenuSystem *menuSystem,
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

    void init() override;

    inline void setCurrentValue(float value) { _currentValue = value; }

    void display() override;

    void onEnterPressed() override;

    void onBackPressed() override;

    void onNextPressed() override;

    void onUpPressed() override;

    void onDownPressed() override;

    std::string getMenuName() override { return _title; }
private:
    std::string _title;
    std::string _unitsDisplay;
    float _minVal = 0;
    float _maxVal = 0;
    float _currentValue = 0;
    float _step = 1.0f;
    RangeEditorCallback _onChangeCallback = nullptr;

};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_RANGEEDITORMENU_H