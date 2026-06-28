
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MCM_CORE0HANDLER_H
#define TECHWAVEAUDIO_MCM_CORE0HANDLER_H
#include <atomic>

#include "CoreHandler.h"
#include "SettingsMenuSystem.h"
#include "pico/flash.h"

extern SystemState *global_system_state;

/**
 * Multicore handler for core 0.
 * Handles the user input, menu and display
 */
class Core0Handler : public CoreHandler {
public:
    explicit Core0Handler(queue_t *inputQueue, queue_t *outputQueue)
        : CoreHandler(inputQueue, outputQueue) {
        _dashboardState.midiChannel = global_system_state->midiChannel;
    }

    ~Core0Handler() override = default;

    /**
     * Sets the menu system. Needed before polling for messages
     * as the dashboard state gets set for the display.
     * @param settingsMenuSystem
     */
    void setMenuSystem(SettingsMenuSystem *settingsMenuSystem) {
        _settingsMenuSystem = settingsMenuSystem;
        _settingsMenuSystem->setDashboardState(&_dashboardState);
    }

    /**
     * Shuts down the system.
     * Signals core 1 to shutdown then waits for an ack message.
     */
    void shutdown();

    /**
     * Sends a signal to core 1 to turn off the output and midi processing
     */
    void turnOffGlobalOutputController() const;

    /**
     * Sends a signal to core 1 to turn on midi and output processing
     */
    void turnOnGlobalOutputController() const;

    /**
     * Gets the current dashboard state settings
     * @return The current dashboard state
     */
    [[nodiscard]] DashboardState getDashboardState() {
        // always set the midi channel here to stay current
        _dashboardState.midiChannel = global_system_state->midiChannel;
        return _dashboardState;
    }

protected:
    void processSignalMessage(SignalCommand command, uint8_t data) override;
    void onAfterProcessEvents() override;

private:
    std::atomic<bool> _waitForAck = false;
    SettingsMenuSystem *_settingsMenuSystem = nullptr;
    DashboardState _dashboardState {};
};


#endif //TECHWAVEAUDIO_MCM_CORE0HANDLER_H