
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "./DashboardDisplay.h"

#include <cstring>
#include <sstream>

#include "GateTrgClockDisplayImageSet.h"
#include "NoteDisplayImageSet.h"

DashboardDisplay::DashboardDisplay(OledDisplay *lcdDisplay, SystemState *systemState) {
    _lcdDisplay = lcdDisplay;
    assert(_lcdDisplay);

    _systemState = systemState;
    assert(_systemState);

    _noteDisplayImageSet = new NoteDisplayImageSet();
    _clockDisplayImageSet = new GateTrgClockDisplayImageSet();
}

DashboardDisplay::~DashboardDisplay() {
    delete _clockDisplayImageSet;
    delete _noteDisplayImageSet;
}

void DashboardDisplay::display() {
    updateInit = false;
    setDefaultTemplate();
    update(true);
}

void DashboardDisplay::update(bool midiSensed) {
    // ReSharper disable once CppDFANullDereference
    if (!_systemState->displayDashboard) {
        if (!_isDashboardCleared) {
            setDefaultTemplate();
        }
        return;
    }

    _isDashboardCleared = false;

    if (midiSensed) {
        if (!_lastMidiSenseStatus) {
            _lcdDisplay->clearArea(_posNoteArea);
        }

        displayMidiChannel();
        displayNote();
        displayVelAuxCtl();
        displayGateTrigger();
        displayClock();
    } else {
        _lcdDisplay->clearArea(_posNoteArea);
        _lcdDisplay->writeTextAt(_posNoteArea.xPos + 8, _posNoteArea.yPos, " no", OledFontType_8x16);
        _lcdDisplay->writeTextAt(_posNoteArea.xPos + 8, _posNoteArea.yPos + 16, "midi", OledFontType_8x16);
    }

    _lastUpdatedState = _currentState;
    _lcdDisplay->show();
    _lastMidiSenseStatus = midiSensed;
    updateInit = true;
}

void DashboardDisplay::setCurrentState(RunningState_t *state) {
    _currentState = *state;
}

void DashboardDisplay::displayMidiChannel() {
    if (updateInit && _currentState.midiChannel == _lastUpdatedState.midiChannel)
        return;

    std::stringstream displayValue;
    if (_currentState.midiChannel == 0) {
        displayValue << "*";
    } else {
        if (_currentState.midiChannel < 10) {
            displayValue << (int)_currentState.midiChannel << " ";
        } else {
            displayValue << (int)_currentState.midiChannel;
        }
    }
    _lcdDisplay->writeTextAt(_posChannelValue.xPos, _posChannelValue.yPos, displayValue.str(), OledFontType_8x16);
}

void DashboardDisplay::displayNote() {
    if (updateInit && _currentState.currentNote == _lastUpdatedState.currentNote)
        return;

    if (_currentState.currentNote == DEFAULT_LAST_NOTE_VALUE) {
        _lcdDisplay->clearArea(_posNoteArea);
        return;
    }

    int octave = (_currentState.currentNote - 12) / 12;
    int whichNote = _currentState.currentNote % 12;
    auto noteDisplayValue = note_names_display[whichNote];

    // images from C (0) to B (6)
    int noteImageIndex = note_image_index[whichNote];
    BoxSize imageSize;
    int imageBytes;
    auto noteImage = _noteDisplayImageSet->getImage(noteImageIndex, imageSize, imageBytes);
    if (noteImage) {
        _lcdDisplay->clearArea(_posNoteName);
        _lcdDisplay->blitImage(_posNoteName.xPos, _posNoteName.yPos, imageSize.width, noteImage, imageBytes);
    }

    _lcdDisplay->clearArea(_posNoteSharpText.xPos, _posNoteSharpText.yPos, 8, 16);
    if (noteDisplayValue.at(1) == '#') {
        _lcdDisplay->writeTextAt(_posNoteSharpText.xPos, _posNoteSharpText.yPos, "#", OledFontType_8x16);
    }

    _lcdDisplay->clearArea(_posNoteOctave.xPos, _posNoteOctave.yPos, 24, 16);
    std::stringstream octaveDisplay;
    octaveDisplay << octave;
    _lcdDisplay->writeTextAt(_posNoteOctave.xPos, _posNoteOctave.yPos, octaveDisplay.str(), OledFontType_8x16);
}

void DashboardDisplay::displayVelAuxCtl() {
    if (!updateInit || _currentState.currentVelocity != _lastUpdatedState.currentVelocity) {
        int velRectWidth = (int)(_barWidthPerPartVel * (float)_currentState.currentVelocity);
        _lcdDisplay->clearArea(_posVelBar.xPos, _posVelBar.yPos, _posVelBar.width, _posVelBar.height);
        _lcdDisplay->drawRect(_posVelBar.xPos, _posVelBar.yPos + 1, velRectWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOut1 != _lastUpdatedState.currentOut1) {
        int auxRectWidth = (int)(_barWidthPerPartAuxCtl * (float)_currentState.currentOut1);
        _lcdDisplay->clearArea(_posAuxBar.xPos, _posAuxBar.yPos, _posAuxBar.width, _posAuxBar.height);
        _lcdDisplay->drawRect(_posAuxBar.xPos, _posAuxBar.yPos + 1, auxRectWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOut2 != _lastUpdatedState.currentOut2) {
        int ctlRectWidth = (int)(_barWidthPerPartAuxCtl * (float)_currentState.currentOut2);
        _lcdDisplay->clearArea(_posCtlBar.xPos, _posCtlBar.yPos, _posCtlBar.width, _posCtlBar.height);
        _lcdDisplay->drawRect(_posCtlBar.xPos, _posCtlBar.yPos + 1, ctlRectWidth, 6, true, true);
    }
}

void DashboardDisplay::displayGateTrigger() {
    BoxSize trgGateImageSize;
    int trgGateImageByteCount;

    if (!updateInit || _currentState.triggerState != _lastUpdatedState.triggerState) {
        auto triggerImage = _clockDisplayImageSet->getImage(
            (_currentState.triggerState ? 3 : 2),
            trgGateImageSize, trgGateImageByteCount
        );

        if (triggerImage) {
            _lcdDisplay->blitImage(_posTriggerArea.xPos, _posTriggerArea.yPos, trgGateImageSize.width, triggerImage, trgGateImageByteCount);
        }
    }

    if (!updateInit || _currentState.gateState != _lastUpdatedState.gateState) {
        auto gateImage = _clockDisplayImageSet->getImage(
            (_currentState.gateState ? 3 : 2),
            trgGateImageSize, trgGateImageByteCount
        );

        if (gateImage) {
            _lcdDisplay->blitImage(_posGateArea.xPos, _posGateArea.yPos, trgGateImageSize.width, gateImage, trgGateImageByteCount);
        }
    }
}

void DashboardDisplay::displayClock() {
    BoxSize clockImageSize;
    int clockImageByteCount;
    if (!updateInit || _currentState.clockState != _lastUpdatedState.clockState) {
        auto clockImage = _clockDisplayImageSet->getImage(
            (_currentState.clockState ? 1 : 0),
            clockImageSize, clockImageByteCount
        );

        if (clockImage) {
            _lcdDisplay->blitImage(_posClockArea.xPos, _posClockArea.yPos, clockImageSize.width, clockImage, clockImageByteCount);
        }
    }
}

void DashboardDisplay::setDefaultTemplate() {
    _lcdDisplay->clear(true);

    _lcdDisplay->writeTextString(_posChannelText.xPos, _posChannelText.yPos, "midi channel:", OledFontType_8x16);
    _lcdDisplay->writeTextString(_posVelText.xPos, _posVelText.yPos, "vel:", OledFontType_8x8);
    _lcdDisplay->writeTextString(_posAuxText.xPos, _posAuxText.yPos, "aux:", OledFontType_8x8);
    _lcdDisplay->writeTextString(_posCtlText.xPos, _posCtlText.yPos, "ctl:", OledFontType_8x8);

    _isDashboardCleared = true;
}
