//
// Created by Mark J. Hoy on 2026-04-30.
//

#include "TimedEventQueue.h"

#include <functional>

#include "TechWaveAudio_MidiControllerModule.h"

TimedEventQueue::~TimedEventQueue() {
    internalClearEvents();
}

void TimedEventQueue::clear() {
    critical_section_enter_blocking(&_queueLock);
    internalClearEvents();
    critical_section_exit(&_queueLock);
}

uint32_t TimedEventQueue::scheduleCallbackEvent(const TimerCallback &callback, long msExpiration) {
    uint32_t eventId = INVALID_EVENT_ID;
    if (callback == nullptr || msExpiration < 0) {
        return eventId;
    }

    critical_section_enter_blocking(&_queueLock);
    eventId = this->insertEvent(callback, GetTicksMs + msExpiration);
    critical_section_exit(&_queueLock);

    return eventId;
}

void TimedEventQueue::removeCallbackEvent(uint32_t eventId) {
    if (eventId == INVALID_EVENT_ID) {
        return;
    }

    critical_section_enter_blocking(&_queueLock);

    auto event = _queueIdMapping.find(eventId);
    if (event != _queueIdMapping.end()) {
        auto node = event->second;
        _queueIdMapping.erase(event);
        if (node == _queueHead) {
            _queueHead = node->next;
            _queueHead->previous = nullptr;
        } else {
            if (node->previous != nullptr) {
                node->previous->next = node->next;
            }
            if (node->next != nullptr) {
                node->next->previous = node->previous;
            }
        }
        _queueSize--;

        delete node;
    }

    critical_section_exit(&_queueLock);
}

void TimedEventQueue::pollAndProcessEvents() {
    TimedEventItem *event = nullptr;
    while ((event = getNextEvent()) != nullptr) {
        event->callback();
        delete event;
    };
}

void TimedEventQueue::pollAndProcessSingleEvent() {
    TimedEventItem *event = getNextEvent();
    if (event != nullptr) {
        event->callback();
        delete event;
    }
}

TimedEventItem *TimedEventQueue::getNextEvent() {
    TimedEventItem * retItem = nullptr;

    // we only need to check the queue head, as we're already in expiration order
    critical_section_enter_blocking(&_queueLock);
    if (_queueHead != nullptr && _queueHead->expirationTime <= GetTicksMs) {
        retItem = _queueHead;
        _queueHead = _queueHead->next;
        if (_queueHead != nullptr) {
            _queueHead->previous = nullptr;
        }
        _queueSize--;
        _queueIdMapping.erase(retItem->eventId);
    }
    critical_section_exit(&_queueLock);

    return retItem;
}

uint32_t TimedEventQueue::insertEvent(const std::function<void()> &callback, uint32_t expirationTime) {
    if (_queueSize >= MAX_ITEMS_IN_EVENT_QUEUE) {
        return INVALID_EVENT_ID;
    }

    _eventId++;
    if (_eventId > MAX_TIMED_EVENT_QUEUE_ID) {
        _eventId = 1;
    }

    auto *item = new TimedEventItem{
        _eventId,
        expirationTime,
        callback,
        nullptr,
        nullptr
    };

    if (_queueHead == nullptr) {
        _queueHead = item;
    } else {
        if (_queueHead->expirationTime > expirationTime) {
            // replace the head
            item->next = _queueHead;
            _queueHead->previous = item;
            _queueHead = item;
        } else {
            // insert after
            TimedEventItem *current = _queueHead;
            while (current->next != nullptr && current->next->expirationTime <= expirationTime) {
                current = current->next;
            }
            if (current->next != nullptr) {
                current->next->previous = item;
            }
            item->previous = current;
            item->next = current->next;
            current->next = item;
        }
    }

    _queueSize++;
    _queueIdMapping.insert(std::pair<uint32_t, TimedEventItem *>(_eventId, item));
    return _eventId;
}

void TimedEventQueue::internalClearEvents() {
    TimedEventItem *current = _queueHead;
    while (current != nullptr) {
        TimedEventItem *nextItem = current->next;
        delete current;
        current = nextItem;
    }
    _queueHead = nullptr;
    _queueIdMapping.clear();
    _queueSize = 0;
}
