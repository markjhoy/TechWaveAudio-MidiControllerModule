//
// Created by Mark J. Hoy on 2026-06-07.
//

#ifndef TECHWAVEAUDIO_MCM_CORE0HANDLER_H
#define TECHWAVEAUDIO_MCM_CORE0HANDLER_H
#include "CoreHandler.h"
#include "SettingsMenuSystem.h"

/**
 * Multicore handler for core 0.
 * Handles the user input, menu and display
 */
class Core0Handler : public CoreHandler {
public:
    explicit Core0Handler(queue_t *inputQueue, queue_t *outputQueue)
        : CoreHandler(inputQueue, outputQueue) {}

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

protected:
    void processSignalMessage(SignalCommand command, uint8_t data) override;
    void onAfterProcessEvents() override;

private:
    std::atomic<bool> _waitForAck = false;
    SettingsMenuSystem *_settingsMenuSystem = nullptr;
    DashboardState _dashboardState {};
};


#endif //TECHWAVEAUDIO_MCM_CORE0HANDLER_H