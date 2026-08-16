
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDIDIAGNOSTICMENU_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDIDIAGNOSTICMENU_H

#include "BaseMenu.h"

#define MAX_MIDI_READ_LOG_MESSAGES 6

class MidiDiagnosticMenu : public BaseMenu {
public:
    MidiDiagnosticMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    ~MidiDiagnosticMenu() override {
        delete[] _logMessages;
    };

    void display() override;

    inline std::string getMenuName() override { return " Midi Read Log"; }

protected:
    void menuInit() override;

    bool onBeforeMenuItemSelected(int menuItemIndex) override;
    bool onMenuItemSelected(int menuItemIndex) override { return false; }

private:
    bool _watchToggle = false;
    int _noteValue = 0;
    int _velocity = 0;
    int _octave = 0;
    std::string _noteDisplay;
    float _pitchBendValue = 0.0f;
    bool _lastClock = false;
    volatile bool _isExiting = false;

    std::string *_logMessages = new std::string[MAX_MIDI_READ_LOG_MESSAGES];
    uint8_t _logCount = -1;

    void updateDisplay(bool refresh = false) const;
    void noteOnCallback(int note, int velocity);
    void noteOffCallback(int noteNumber, int _);
    void allNotesOffCallback();
    void onModWheelCallback(int data);
    void onPitchBendCallback(int fineValue, int coarseValue);
    void onSustainCallback(int data);
    void onVolumeCallback(int velocity);
    void onAftertouchCallback(int data);
    void onExpressionCallback(int data);
    void onEffectOneCallback(int data);
    void onEffectTwoCallback(int data);
    void onResetCallback();
    void onClockCallback();

    void addLogMessage(const std::string &message);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDIDIAGNOSTICMENU_H