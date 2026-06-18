//
// Created by Mark J. Hoy on 2026-04-30.
//

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H

#include "../TechWaveAudio_MidiControllerModule.h"
#include "../TimedEventQueue.h"
#include "hardware/gpio.h"

class Button {
public:
    Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber);
    Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber, uint32_t bounceTime);

    ~Button() = default;

    void setOnPressed(const GeneralFunctionCallback &callback);
    void setOnReleased(const GeneralFunctionCallback &callback);

    [[nodiscard]] uint8_t getPinNumber() const { return _pinNumber; }

    void onButtonPressed(uint32_t events);
    void shutdown() const;

private:
    TimedEventQueue *_timedEventQueue = nullptr;
    uint8_t _pinNumber = 0;

    GeneralFunctionCallback _onPressedCallback = nullptr;
    GeneralFunctionCallback _onReleasedCallback = nullptr;
    const uint32_t _switchEventMask = GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE;

#ifndef USE_HARDWARE_DEBOUNCE
    bool _lastActiveState = false;
    uint32_t _bounceTimeMs = 0L;
    uint32_t _debounceExpirationMs = 0;
#endif

    void setupButton(int pinNumber, uint32_t bounceTime);
};

void global_button_registry_init();

#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_BUTTON_H