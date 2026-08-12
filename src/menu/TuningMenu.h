
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_TUNINGMENU_H
#define TECHWAVEAUDIO_MCM_TUNINGMENU_H
#include <atomic>

#include "BaseMenu.h"

#define TUNING_MENU_SELECTION_A4 0
#define TUNING_MENU_SELECTION_C4 1
#define TUNING_MENU_SELECTION_C0 2
#define TUNING_MENU_SELECTION_C1 3
#define TUNING_MENU_SELECTION_C2 4
#define TUNING_MENU_SELECTION_C3 5
#define TUNING_MENU_SELECTION_C5 6
#define TUNING_MENU_SELECTION_C6 7
#define TUNING_MENU_SELECTION_C7 8
#define TUNING_MENU_SELECTION_C8 9
#define TUNING_MENU_SELECTION_C9 10
#define TUNING_MENU_NUM_SELECTIONS 11

class NoteVelOut1Out2Output;
class OutputController;
class Mcp4725;

class TuningMenu : public BaseMenu {
public:
    TuningMenu(OledDisplay *lcdDisplay, IMenuSystemHandler *menuSystem, SystemState *systemState, BaseMenu *previousMenu)
        : BaseMenu(lcdDisplay, menuSystem, systemState, previousMenu) {
    }

    void display() override;

    std::string getMenuName() override { return "     Tuning"; }

protected:
    void menuInit() override;

    bool onMenuItemSelected(int menuItemIndex) override;

    bool onBeforeMenuItemSelected(int menuItemIndex) override;

    bool onBeforeLeftRotation(int currentMenuItemIndex) override;

    bool onBeforeRightRotation(int currentMenuItemIndex) override;

private:
    OutputController *_outputController = nullptr;

    NoteVelOut1Out2Output *_outputDac = nullptr;
    std::atomic<bool> _isTuning = false;
    std::atomic<bool> _isClosing = false;
    int _selectedChoice = 0;
    std::vector<std::string> _choices;
    std::vector<uint8_t> _choiceNoteValues;

    void reset();
    void performTuning(const std::string& noteName, uint8_t noteValue);
};


#endif //TECHWAVEAUDIO_MCM_TUNINGMENU_H