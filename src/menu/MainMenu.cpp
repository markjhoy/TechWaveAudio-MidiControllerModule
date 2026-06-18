//
// Created by Mark J. Hoy on 2026-05-02.
//

#include "MainMenu.h"

#include "../SettingsMenuSystem.h"
#include "RangeEditorMenu.h"
#include "TuningMenu.h"

static std::string main_menu_selections[] = {
    "Midi Channel",
    "Note Priority",
    "Pitch Adj",
    "Velocity Adj",
    "Pitch Bend Range",
    "Aux Output",
    "Control Output",
    "Trigger Duration",
    "Output Voltages",
    "Display Options",
    "Tuning Menu",
    "Calibration",
    "About",
    "Reset All",
    "Exit"
};

MainMenu::MainMenu(OledDisplay *lcdDisplay, SettingsMenuSystem *menuSystem, SystemState *systemState)
: BaseMenu(lcdDisplay, menuSystem, systemState, nullptr)
{
    setupMenus();
}

MainMenu::~MainMenu() {
    delete _pitchAdjustMenu;
    delete _midiChannelMenu;
    delete _pitchbendRangeMenu;
    delete _controlOutputMenu;
    delete _auxOutputMenu;
    delete _triggerDurationMenu;
    delete _velocityAdjustMenu;
    delete _notePriorityMenu;
    delete _outputVoltageSelectMenu;
    delete _displaySettingsMenu;
    delete _tuningMenu;
    delete _calibrationMenu;
    delete _aboutMenu;
    delete _resetMenu;
}

void MainMenu::init() {
    _currentMenuItem = 0;
}

void MainMenu::display() {
    _lcdDisplay->showMenu("   Settings", main_menu_selections, _currentMenuItem, MAIN_MENU_NUM_ITEMS);
}

void MainMenu::onEnterPressed() {
    switch (_currentMenuItem) {
        case MAIN_MENU_MIDI_CH: {
            _menuSystem->changeMenu(_midiChannelMenu);
        } break;
        case MAIN_MENU_PITCH_ADJ: {
            _pitchAdjustMenu->setCurrentValue((float)_systemState->pitchAdjust);
            _menuSystem->changeMenu(_pitchAdjustMenu);
        } break;
        case MAIN_MENU_VELOCITY_ADJ:  {
            _velocityAdjustMenu->setCurrentValue((float)_systemState->velocityAdjust);
            _menuSystem->changeMenu(_velocityAdjustMenu);
        } break;
        case MAIN_MENU_NOTE_PRIORITY: {
            _menuSystem->changeMenu(_notePriorityMenu);
        };break;
        case MAIN_MENU_PITCH_BEND_RANGE: {
            _pitchbendRangeMenu->setCurrentValue(_systemState->pitchBendRange);
            _menuSystem->changeMenu(_pitchbendRangeMenu);
        } break;
        case MAIN_MENU_AUX_OUTPUT: {
            _menuSystem->changeMenu(_auxOutputMenu);
        } break;
        case MAIN_MENU_CTL_OUTPUT: {
            _menuSystem->changeMenu(_controlOutputMenu);
        } break;
        case MAIN_MENU_TRIGGER_DUR: {
            _menuSystem->changeMenu(_triggerDurationMenu);
        } break;
        case MAIN_MENU_OUTPUT_VOLTAGES: {
            _menuSystem->changeMenu(_outputVoltageSelectMenu);
        } break;
        case MAIN_MENU_DISPLAY_OPTIONS: {
            _menuSystem->changeMenu(_displaySettingsMenu);
        } break;
        case MAIN_MENU_TUNING: {
            _menuSystem->changeMenu(_tuningMenu);
        } break;
        case MAIN_MENU_CALIBRATION: {
            _menuSystem->changeMenu(_calibrationMenu);
        } break;
        case MAIN_MENU_ABOUT: {
            _menuSystem->changeMenu(_aboutMenu);
        } break;
        case MAIN_MENU_RESET_ALL: {
            _menuSystem->changeMenu(_resetMenu);
        } break;
        case MAIN_MENU_EXIT: {
            _menuSystem->setShouldExit();
            _menuSystem->changeMenu(nullptr);
        } break;
    }
}

void MainMenu::onBackPressed() {
    _menuSystem->changeMenu(nullptr);
}

void MainMenu::onNextPressed() {
    // do nothing
}

void MainMenu::onUpPressed() {
    _currentMenuItem -= 1;
    if (_currentMenuItem < 0) {
        _currentMenuItem = MAIN_MENU_NUM_ITEMS - 1;
    }
    display();
}

void MainMenu::onDownPressed() {
    _currentMenuItem += 1;
    if (_currentMenuItem >= MAIN_MENU_NUM_ITEMS) {
        _currentMenuItem = 0;
    }
    display();
}

void MainMenu::onPitchAdjustChange(float value) const {
    _systemState->pitchAdjust = value;
}

void MainMenu::onPitchbendAdjustChange(float value) const {
    _systemState->pitchBendRange = value;
}

void MainMenu::onVelocityAdjustChange(float value) const {
    _systemState->velocityAdjust = static_cast<int>(value);
}

void MainMenu::setupMenus() {
    _pitchAdjustMenu = new RangeEditorMenu(
        _lcdDisplay,
        _menuSystem,
        _systemState,
        this,
        "Pitch Adjust",
         "  (raw value)",
        MIN_PITCH_ADJUST,
        MAX_PITCH_ADJUST,
        [this](auto && PH1) { onPitchAdjustChange(std::forward<decltype(PH1)>(PH1)); },
        1.0f
    );
    _midiChannelMenu = new MidiChannelMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _pitchbendRangeMenu = new RangeEditorMenu(
        _lcdDisplay,
        _menuSystem,
        _systemState,
        this,
        "Pitch Bend Range",
        "  (in octaves)",
        0.0f,
        MAX_PITCH_BEND_RANGE_OCTAVES,
        [this](auto && PH1) { onPitchbendAdjustChange(std::forward<decltype(PH1)>(PH1)); },
        0.25f
    );
    _auxOutputMenu = new AuxOutputMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _controlOutputMenu = new ControlOutputMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _triggerDurationMenu = new TriggerDurationMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _velocityAdjustMenu = new RangeEditorMenu(
        _lcdDisplay,
        _menuSystem,
        _systemState,
        this,
        "Velocity Adjust",
         "  (raw value)",
        MIN_VELOCITY_ADJUST,
        MAX_VELOCITY_ADJUST,
        [this](auto && PH1) { onVelocityAdjustChange(std::forward<decltype(PH1)>(PH1)); },
        1.0f
    );
    _notePriorityMenu = new NotePriorityMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _outputVoltageSelectMenu = new OutputVoltageSelectMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _displaySettingsMenu = new DisplaySettingsMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _tuningMenu = new TuningMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _calibrationMenu = new CalibrationMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _aboutMenu = new AboutMenu(_lcdDisplay,_menuSystem,_systemState,this);
    _resetMenu = new ResetMenu(_lcdDisplay,_menuSystem,_systemState,this);
}
