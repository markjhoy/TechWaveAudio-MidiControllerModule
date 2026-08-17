
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "GpioPinEventHandler.h"

#include <map>
#include <functional>
#include "hardware/gpio.h"
#include "pico/critical_section.h"

std::map<uint8_t, GpioPinEventHandler*> global_gpio_event_registry;
critical_section global_gpio_event_registry_lock;

/**
 * Initializes the global registry
 */
void global_gpio_event_registry_init() {
    critical_section_init(&global_gpio_event_registry_lock);
}

/**
 * Adds a GPIO pin to the global registry with a callback
 * @param pin the pin of the GPIO input
 */
void global_gpio_event_registry_add(GpioPinEventHandler* pin) {
    critical_section_enter_blocking(&global_gpio_event_registry_lock);
    global_gpio_event_registry.insert(std::pair<uint8_t, GpioPinEventHandler*>(pin->getPinNumber(), pin));
    critical_section_exit(&global_gpio_event_registry_lock);
}

/**
 * Removes a GPIO pin from the global registry
 * @param pin the pin of the GPIO input
 */
void global_gpio_event_registry_remove(const GpioPinEventHandler *pin) {
    critical_section_enter_blocking(&global_gpio_event_registry_lock);
    auto it = global_gpio_event_registry.find(pin->getPinNumber());
    if (it != global_gpio_event_registry.end()) {
        global_gpio_event_registry.erase(it);
    }
    critical_section_exit(&global_gpio_event_registry_lock);
}

/**
 * Handler for a change in a GPIO pin state
 * @param pinId the pin that raised the event
 * @param events the event flags
 */
void global_gpio_event_irq_callback(uint pinId, uint32_t events) {
    auto eventHandler = global_gpio_event_registry.find(pinId);
    if (eventHandler == global_gpio_event_registry.end()) {
        return;
    }
    eventHandler->second->onPinChange(events);
}

/**
 * Enables IRQ handling for a GPIO input
 * @param pinId the GPIO pin
 */
void global_gpio_event_irq_enable(uint8_t pinId) {
    critical_section_enter_blocking(&global_gpio_event_registry_lock);
    gpio_set_irq_enabled_with_callback(pinId, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, global_gpio_event_irq_callback);
    critical_section_exit(&global_gpio_event_registry_lock);
}

/**
 * Disables IRQ handling for a GPIO input
 * @param pinId the GPIO pin
 */
void global_gpio_event_irq_disable(uint8_t pinId) {
    critical_section_enter_blocking(&global_gpio_event_registry_lock);
    gpio_set_irq_enabled(pinId, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, false);
    critical_section_exit(&global_gpio_event_registry_lock);
}

GpioPinEventHandler::GpioPinEventHandler(uint8_t pinNumber, const OnPinValueChangeCallback &callback) {
    this->_pinNumber = pinNumber;
    this->_onPinValueChangeCallback = callback;
    global_gpio_event_registry_add(this);
    global_gpio_event_irq_enable(this->_pinNumber);
}

GpioPinEventHandler::~GpioPinEventHandler() {
    global_gpio_event_irq_disable(this->_pinNumber);
    global_gpio_event_registry_remove(this);
}

void GpioPinEventHandler::onPinChange(uint32_t events) {
    if (!(events & _switchEventMask)) {
        return;
    }

    if (_onPinValueChangeCallback == nullptr) {
        return;
    }

    this->_onPinValueChangeCallback(this->_pinNumber, events & GPIO_IRQ_EDGE_RISE);
}
