
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#include "MidiAndOutputHandler.h"
#include "GlobalHandlers.h"

MidiAndOutputHandler::MidiAndOutputHandler(queue_t *inputQueue, queue_t *outputQueue)
: CoreHandler(inputQueue, outputQueue) {
    _keepRunning = true;
    _eventQueue = new TimedEventQueue();
    _outputController = new OutputController(global_system_state, _eventQueue, this->getMultiCoreController());
}

MidiAndOutputHandler::~MidiAndOutputHandler() {
    delete _outputController;
    delete _eventQueue;
}

void MidiAndOutputHandler::processSignalMessage(SignalCommand command, uint8_t data) {
    switch (command) {
        case SignalCommand_OutputOn: {
            gpio_put(PIN_CLOCK_LED, true);
            _outputController->init();
        } break;
        case SignalCommand_OutputOff: {
            _outputController->shutdown();
        } break;
        case SignalCommand_Shutdown: {
            _outputController->shutdown();
            this->_keepRunning = false;
            this->sendSignal(SignalCommand_Shutdown_Ack, 0);
        } break;
        default: {}
    }
}

void MidiAndOutputHandler::onAfterProcessEvents() {
    _eventQueue->pollAndProcessEvents();
}
