
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

    if (_systemState->expansionSensed) {
        _setVelText = _posExVelText;
        _setVelBar = _posExVelBar;
        _setOut1Text = _posExOut1Text;
        _setOut1Bar = _posExOut1Bar;
        _setOut2Text = _posExOut2Text;
        _setOut2Bar = _posExOut2Bar;
        _setTriggerArea = _posExTriggerArea;
        _setGateArea = _posExGateArea;
        _setClockArea = _posExClockArea;
    } else {
        _setVelText = _posVelText;
        _setVelBar = _posVelBar;
        _setOut1Text = _posOut1Text;
        _setOut1Bar = _posOut1Bar;
        _setOut2Text = _posOut2Text;
        _setOut2Bar = _posOut2Bar;
        _setTriggerArea = _posTriggerArea;
        _setGateArea = _posGateArea;
        _setClockArea = _posClockArea;
    }

    _barWidthPerPartVel = static_cast<float>(_setVelBar.width) / 127.0f;
    _barWidthPerPartOut1 = static_cast<float>(_setOut1Bar.width) / 255.0f;
    _barWidthPerPartOut2 = static_cast<float>(_setOut2Bar.width) / 255.0f;
    _barWidthPerPartXOut = static_cast<float>(_posExOutX1Bar.width) / 255.0f;
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
        displayVelOut1Out2();
        displayExOut();
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

void DashboardDisplay::displayVelOut1Out2() {
    if (!updateInit || _currentState.currentVelocity != _lastUpdatedState.currentVelocity) {
        int outWidth = static_cast<int>(_barWidthPerPartVel * static_cast<float>(_currentState.currentVelocity));
        _lcdDisplay->clearArea(_setVelBar.xPos, _setVelBar.yPos, _setVelBar.width, _setVelBar.height);
        _lcdDisplay->drawRect(_setVelBar.xPos, _setVelBar.yPos + 1, outWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOut1 != _lastUpdatedState.currentOut1) {
        int outWidth = static_cast<int>(_barWidthPerPartOut1 * static_cast<float>(_currentState.currentOut1));
        _lcdDisplay->clearArea(_setOut1Bar.xPos, _setOut1Bar.yPos, _setOut1Bar.width, _setOut1Bar.height);
        _lcdDisplay->drawRect(_setOut1Bar.xPos, _setOut1Bar.yPos + 1, outWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOut2 != _lastUpdatedState.currentOut2) {
        int outWidth = static_cast<int>(_barWidthPerPartOut2 * static_cast<float>(_currentState.currentOut2));
        _lcdDisplay->clearArea(_setOut2Bar.xPos, _setOut2Bar.yPos, _setOut2Bar.width, _setOut2Bar.height);
        _lcdDisplay->drawRect(_setOut2Bar.xPos, _setOut2Bar.yPos + 1, outWidth, 6, true, true);
    }
}

void DashboardDisplay::displayExOut() {
    if (_currentState.expansionSensed == false)
        return;

    if (!updateInit || _currentState.currentOutX1 != _lastUpdatedState.currentOutX1) {
        int outWidth = static_cast<int>(_barWidthPerPartXOut * static_cast<float>(_currentState.currentOutX1));
        _lcdDisplay->clearArea(_posExOutX1Bar.xPos, _posExOutX1Bar.yPos, _posExOutX1Bar.width, _posExOutX1Bar.height);
        _lcdDisplay->drawRect(_posExOutX1Bar.xPos, _posExOutX1Bar.yPos + 1, outWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOutX2 != _lastUpdatedState.currentOutX2) {
        int outWidth = static_cast<int>(_barWidthPerPartXOut * static_cast<float>(_currentState.currentOutX1));
        _lcdDisplay->clearArea(_posExOutX2Bar.xPos, _posExOutX2Bar.yPos, _posExOutX2Bar.width, _posExOutX2Bar.height);
        _lcdDisplay->drawRect(_posExOutX2Bar.xPos, _posExOutX2Bar.yPos + 1, outWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOutX3 != _lastUpdatedState.currentOutX3) {
        int outWidth = static_cast<int>(_barWidthPerPartXOut * static_cast<float>(_currentState.currentOutX1));
        _lcdDisplay->clearArea(_posExOutX3Bar.xPos, _posExOutX3Bar.yPos, _posExOutX3Bar.width, _posExOutX3Bar.height);
        _lcdDisplay->drawRect(_posExOutX3Bar.xPos, _posExOutX3Bar.yPos + 1, outWidth, 6, true, true);
    }

    if (!updateInit || _currentState.currentOutX4 != _lastUpdatedState.currentOutX4) {
        int outWidth = static_cast<int>(_barWidthPerPartXOut * static_cast<float>(_currentState.currentOutX1));
        _lcdDisplay->clearArea(_posExOutX4Bar.xPos, _posExOutX4Bar.yPos, _posExOutX4Bar.width, _posExOutX4Bar.height);
        _lcdDisplay->drawRect(_posExOutX4Bar.xPos, _posExOutX4Bar.yPos + 1, outWidth, 6, true, true);
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
            _lcdDisplay->blitImage(_setTriggerArea.xPos, _setTriggerArea.yPos, trgGateImageSize.width, triggerImage, trgGateImageByteCount);
        }
    }

    if (!updateInit || _currentState.gateState != _lastUpdatedState.gateState) {
        auto gateImage = _clockDisplayImageSet->getImage(
            (_currentState.gateState ? 3 : 2),
            trgGateImageSize, trgGateImageByteCount
        );

        if (gateImage) {
            _lcdDisplay->blitImage(_setGateArea.xPos, _setGateArea.yPos, trgGateImageSize.width, gateImage, trgGateImageByteCount);
        }
    }
}

void DashboardDisplay::displayClock() {
    BoxSize clockImageSize;
    int clockImageByteCount;
    uint8_t *clockImage;
    if (!updateInit || _currentState.clockState != _lastUpdatedState.clockState) {
        if (_currentState.expansionSensed) {
            clockImage = _clockDisplayImageSet->getImage(
                (_currentState.clockState ? 3 : 2),
                clockImageSize, clockImageByteCount
            );
        } else {
            clockImage = _clockDisplayImageSet->getImage(
                (_currentState.clockState ? 1 : 0),
                clockImageSize, clockImageByteCount
            );
        }

        if (clockImage) {
            _lcdDisplay->blitImage(_setClockArea.xPos, _setClockArea.yPos, clockImageSize.width, clockImage, clockImageByteCount);
        }
    }
}

void DashboardDisplay::setDefaultTemplate() {
    _lcdDisplay->clear(true);

    _lcdDisplay->writeTextString(_posChannelText.xPos, _posChannelText.yPos, "midi channel:", OledFontType_8x16);

    if (_systemState->expansionSensed) {
        _lcdDisplay->writeTextString(_posVelText.xPos, _posVelText.yPos, " v", OledFontType_8x8);
        _lcdDisplay->writeTextString(_setOut1Text.xPos, _setOut1Text.yPos, " 1", OledFontType_8x8);
        _lcdDisplay->writeTextString(_setOut2Text.xPos, _setOut2Text.yPos, " 2", OledFontType_8x8);
        _lcdDisplay->writeTextString(_posExOutX1Text.xPos, _posExOutX1Text.yPos, "x1", OledFontType_8x8);
        _lcdDisplay->writeTextString(_posExOutX2Text.xPos, _posExOutX2Text.yPos, "x2", OledFontType_8x8);
        _lcdDisplay->writeTextString(_posExOutX3Text.xPos, _posExOutX3Text.yPos, "x3", OledFontType_8x8);
        _lcdDisplay->writeTextString(_posExOutX4Text.xPos, _posExOutX4Text.yPos, "x4", OledFontType_8x8);
    } else {
        _lcdDisplay->writeTextString(_posVelText.xPos, _posVelText.yPos, "vel:", OledFontType_8x8);
        _lcdDisplay->writeTextString(_setOut1Text.xPos, _setOut1Text.yPos, "o1:", OledFontType_8x8);
        _lcdDisplay->writeTextString(_setOut2Text.xPos, _setOut2Text.yPos, "o2:", OledFontType_8x8);
    }

    _isDashboardCleared = true;
}
