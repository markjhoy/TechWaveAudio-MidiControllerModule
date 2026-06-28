
/*******************************************************************************
 * Copyright (c) 2026 TechWave Audio (techwaveaudio.com)
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 ******************************************************************************/

#ifndef TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TIMEDEVENTQUEUE_H
#define TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TIMEDEVENTQUEUE_H
#include <functional>
#include <map>

#include "TechWaveAudio_MidiControllerModule.h"
#include "hardware/sync.h"

#define MAX_TIMED_EVENT_QUEUE_ID 2048000000
#define MAX_ITEMS_IN_EVENT_QUEUE 8192

/**
 * A thread-safe preemptive scheduling priority queue.
 * Keeps items in order of when they expire and should be called back.
 * Callbacks should not run long-running tasks.
 *
 * Underneath the hood, a linked list is kept in order of the next event
 * that is ready to be executed.
 */
class TimedEventQueue {
public:
    TimedEventQueue();
    ~TimedEventQueue();

    /**
     * Clears any events in the queue
     */
    void clear();

    /**
     * Schedules an event to be called back in a certain number of milliseconds
     * @param callback the callback function to execute
     * @param msExpiration the number of milliseconds to wait before calling the callback
     * @return the event ID (can be used to remove the event before it is fired)
     */
    uint32_t scheduleCallbackEvent(const TimerCallback &callback, long msExpiration);

    /**
     * Removes a single event from the queue. If the event does not exist, nothing is changed.
     * @param eventId the id of the event to remove
     */
    void removeCallbackEvent(uint32_t eventId);

    /**
     * Processes all available events that are at or past their time to execute
     */
    void pollAndProcessEvents();

    /**
     * Processes the next event (if any) that is past their time to execute.
     */
    void pollAndProcessSingleEvent();
private:
    std::map<uint32_t, TimedEventItem *> _queueIdMapping;
    TimedEventItem_t *_queueHead = nullptr;
    uint32_t _queueSize = 0;
    uint32_t _eventId = 0;

    int _queueLock_lockNum = 0;
    spin_lock_t *_queueLock_spinlock;

    void internalClearEvents();
    TimedEventItem *getNextEvent();

    uint32_t insertEvent(const TimerCallback &callback, uint32_t expirationTime);
};


#endif // TECHWAVEAUDIO_MIDI_CONTROLLER_MODULE_TIMEDEVENTQUEUE_H