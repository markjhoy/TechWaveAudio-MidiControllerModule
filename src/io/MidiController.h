/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H
#include <cstdint>
#include <functional>
#include "../TechWaveAudio_MidiControllerModule.h"
#include "pico/sem.h"

/**
 * Our MIDI input controller.
 * Waits for messages to be sent to the `runCommand` method and routes those to the callback handlers.
 */
class MidiController {
public:
    explicit MidiController(uint8_t channel=DEFAULT_MIDI_CHANNEL);
    ~MidiController();

    /**
     * Starts watching for incoming MIDI messages
     */
    void start();

    /**
     * Stops watching for incoming MIDI messages
     */
    void stop();

    /**
     * Sets the channel to watch for MIDI messages
     * @param newChannel the channel to watch for incoming messages on. 0 = all channels
     */
    void setChannel(int newChannel);

    /**
     * Pause the processing of any new commands.
     * Any read incoming messages are discarded.
     */
    void pause();

    /**
     * Resumes the processing of commands.
     */
    void resume();

    /**
     * Mutes or unmutes any note output commands.
     * @param mute to mute note output or not
     */
    void setMute(bool mute);

    /**
     * Runs a midi command that was received.
     * @param message the incoming MIDI message
     */
    void runCommand(const MidiMessage &message);

    [[nodiscard]] inline bool isRunning() const { return _isStarted; }
    [[nodiscard]] inline bool isPaused() const { return _isPaused; }

    /**
     * Sets the callback for when a note on command is received
     * @param callback the callback function
     */
    void setOnNoteOnCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onNoteOnCallback = callback; }

    /**
    * Sets the callback for when a note off command is received
    * @param callback the callback function
    */
    void setOnNoteOffCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onNoteOffCallback = callback; }

    /**
    * Sets the callback for when an all notes off command is received
    * @param callback the callback function
    */
    void setOnAllNotesOffCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onAllNotesOffCallback = callback; }

    /**
    * Sets the callback for when a mod wheel command is received
    * @param callback the callback function
    */
    void setOnModWheelCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onModWheelCallback = callback; }

    /**
    * Sets the callback for when a pitch bend command is received
    * @param callback the callback function
    */
    void setOnPitchBendCallback(const DoubleValueMidiMessageCallback &callback) { if (!_isStarted) _onPitchBendCallback = callback; }

    /**
    * Sets the callback for when a sustain command is received
    * @param callback the callback function
    */
    void setOnSustainCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onSustainCallback = callback; }

    /**
    * Sets the callback for when a volume (velocity) change command is received
    * @param callback the callback function
    */
    void setOnVolumeChangedCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onVolumeChangeCallback = callback; }

    /**
    * Sets the callback for when an aftertouch command is received
    * @param callback the callback function
    */
    void setOnAftertouchCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onAftertouchCallback = callback; }

    /**
    * Sets the callback for when an expression command is received
    * @param callback the callback function
    */
    void setOnExpressionCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onExpressionCallback = callback; }

    /**
    * Sets the callback for when an effect one command is received
    * @param callback the callback function
    */
    void setOnEffectOneCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onEffectOneCallback = callback; }

    /**
    * Sets the callback for when an effect two command is received
    * @param callback the callback function
    */
    void setOnEffectTwoCallback(const SingleValueMidiMessageCallback &callback) { if (!_isStarted) _onEffectTwoCallback = callback; }

    /**
    * Sets the callback for when a global reset command is received
    * @param callback the callback function
    */
    void setOnResetCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onResetCallback = callback; }

    /**
    * Sets the callback for when a clock tick is received
    * @param callback the callback function
    */
    void setOnClockCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onClockCallback = callback; }

    void setOnStartCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onStartCallback = callback; }
    void setOnStopCallback(const NoValueMidiMessageCallback &callback) { if (!_isStarted) _onStopCallback = callback; }

private:
    volatile uint8_t _midiChannel = DEFAULT_MIDI_CHANNEL;
    volatile bool _isStarted = false;
    volatile bool _isPaused = false;
    volatile bool _muteAll = false;
    semaphore_t _commandLock{};

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
    NoValueMidiMessageCallback _onStartCallback;
    NoValueMidiMessageCallback _onStopCallback;
};

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_MIDICONTROLLER_H