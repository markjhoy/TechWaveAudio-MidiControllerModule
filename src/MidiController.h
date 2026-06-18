//
// Created by Mark J. Hoy on 2026-05-03.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H
#include <cstdint>
#include <functional>
#include "TechWaveAudio_MidiControllerModule.h"

class MidiController {
public:
    explicit MidiController(uint8_t channel=DEFAULT_MIDI_CHANNEL);
    ~MidiController();

    void start();
    void stop();
    void setChannel(int newChannel);
    void pause();
    void resume();
    void setMute(bool mute);
    void runCommand(const MidiMessage &message) const;

    [[nodiscard]] inline bool isRunning() const { return _isStarted; }
    [[nodiscard]] inline bool isPaused() const { return _isPaused; }

    void setOnNoteOnCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onNoteOnCallback = callback; }
    void setOnNoteOffCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onNoteOffCallback = callback; }
    void setOnAllNotesOffCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onAllNotesOffCallback = callback; }
    void setOnModWheelCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onModWheelCallback = callback; }
    void setOnPitchBendCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onPitchBendCallback = callback; }
    void setOnSustainCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onSustainCallback = callback; }
    void setOnVolumeChangedCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onVolumeChangeCallback = callback; }
    void setOnAftertouchCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onAftertouchCallback = callback; }
    void setOnExpressionCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onExpressionCallback = callback; }
    void setOnEffectOneCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onEffectOneCallback = callback; }
    void setOnEffectTwoCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onEffectTwoCallback = callback; }
    void setOnResetCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onResetCallback = callback; }
    void setOnClockCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onClockCallback = callback; }

private:
    volatile uint8_t _midiChannel = DEFAULT_MIDI_CHANNEL;
    volatile bool _isStarted = false;
    volatile bool _isPaused = false;
    volatile bool _muteAll = false;

    DoubleValueMidiMessageCallback _onNoteOnCallback;
    DoubleValueMidiMessageCallback _onNoteOffCallback;
    NoValueMidiMessageCallback _onAllNotesOffCallback;
    SingleValueMidiMessageCallback _onModWheelCallback;
    DoubleValueMidiMessageCallback _onPitchBendCallback;
    SingleValueMidiMessageCallback _onSustainCallback;
    SingleValueMidiMessageCallback _onVolumeChangeCallback;
    SingleValueMidiMessageCallback _onAftertouchCallback;
    SingleValueMidiMessageCallback _onExpressionCallback;
    SingleValueMidiMessageCallback _onEffectOneCallback;
    SingleValueMidiMessageCallback _onEffectTwoCallback;
    NoValueMidiMessageCallback _onResetCallback;
    NoValueMidiMessageCallback _onClockCallback;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H