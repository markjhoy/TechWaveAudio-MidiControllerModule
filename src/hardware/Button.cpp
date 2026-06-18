//
// Created by Mark J. Hoy on 2026-04-30.
//

#include "Button.h"

#include <functional>
#include "hardware/gpio.h"

std::map<uint8_t, Button*> global_button_registry;
critical_section global_button_registry_lock;

void global_button_registry_init() {
    critical_section_init(&global_button_registry_lock);
}

void global_button_registry_add_button(Button* button) {
    critical_section_enter_blocking(&global_button_registry_lock);
    global_button_registry.insert(std::pair<uint8_t, Button*>(button->getPinNumber(), button));
    critical_section_exit(&global_button_registry_lock);
}

void global_button_registry_remove_button(const Button *button) {
    critical_section_enter_blocking(&global_button_registry_lock);
    auto it = global_button_registry.find(button->getPinNumber());
    if (it != global_button_registry.end()) {
        global_button_registry.erase(it);
    }
    critical_section_exit(&global_button_registry_lock);
}

void global_button_irq_callback(uint pinId, uint32_t events) {
    auto button = global_button_registry.find(pinId);
    if (button == global_button_registry.end()) {
        return;
    }
    button->second->onButtonPressed(events);
}

void global_button_irq_enable(uint8_t pinId) {
    critical_section_enter_blocking(&global_button_registry_lock);
    gpio_set_irq_enabled_with_callback(pinId, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, global_button_irq_callback);
    critical_section_exit(&global_button_registry_lock);
}

void global_button_irq_disable(uint8_t pinId) {
    critical_section_enter_blocking(&global_button_registry_lock);
    gpio_set_irq_enabled(pinId, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, false);
    critical_section_exit(&global_button_registry_lock);
}

Button::Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber) {
    _timedEventQueue = timedEventQueue;
    setupButton(pinNumber, 0L);
}

Button::Button(TimedEventQueue *timedEventQueue, uint8_t pinNumber, uint32_t bounceTime) {
    _timedEventQueue = timedEventQueue;
    setupButton(pinNumber, bounceTime);
}

void Button::setOnPressed(const GeneralFunctionCallback &callback) {
    this->_onPressedCallback = callback;
}

void Button::setOnReleased(const GeneralFunctionCallback &callback) {
    this->_onReleasedCallback = callback;
}

void Button::setupButton(int pinNumber, uint32_t bounceTime) {
    this->_pinNumber = pinNumber;

#ifndef USE_HARDWARE_DEBOUNCE
    this->_bounceTimeMs = bounceTime;
    _debounceExpirationMs = 0;
#endif

    global_button_registry_add_button(this);
    global_button_irq_enable(this->_pinNumber);
}

void Button::onButtonPressed(uint32_t events) {
    if (!(events & _switchEventMask)) {
        return;
    }

#ifdef USE_HARDWARE_DEBOUNCE
    // hardware debounce inverts this signal
    // so pressed is high, released is low
    if (gpio_get(_pinNumber) && _onPressedCallback != nullptr) {
        _onPressedCallback();
    } else if (_onReleasedCallback != nullptr) {
        _onReleasedCallback();
    }
#else
    if (_debounceExpirationMs == 0) {
        _debounceExpirationMs = GetTicksMs + _bounceTimeMs;
    }

    if (GetTicksMs > _debounceExpirationMs) {
        _debounceExpirationMs = 0;
        bool pinValue = gpio_get(_pinNumber);
        if (pinValue) {
            // was released
            if (_onReleasedCallback != nullptr) {
                _onReleasedCallback();
            }
        } else {
            // was pressed
            if (_onPressedCallback != nullptr) {
                _onPressedCallback();
            }
        }
    }
#endif
}

void Button::shutdown() const {
    global_button_irq_enable(this->_pinNumber);
    global_button_registry_remove_button(this);
}

