//
// Created by Weihang Xiao on 3/4/20.
//

#include "DESLayer.h"

void DESLayer::addEvent(Event* e) {
    list<Event*>::iterator it;
    for (it = eventQueue.begin(); !eventQueue.empty() && it != eventQueue.end(); ++it) {
        if (e->getEvtTimestamp() < (*it)->getEvtTimestamp()) {
            break;
        }
    }
    eventQueue.insert(it, e);

}

void DESLayer::printBefEvtQueue(Event* e) {
    printf("AddEvent(%d:%d:%s):  ", e->getEvtTimestamp(), e->getEvtProcess(), converTransition(e->getTransition()).c_str());
    list<Event*>::iterator it;
    for (it = eventQueue.begin(); !eventQueue.empty() && it != eventQueue.end(); ++it) {
        printf("%d:%d:%s ", (*it)->getEvtTimestamp(), (*it)->getEvtProcess(), converTransition((*it)->getTransition()).c_str());
    }
}

int DESLayer::getExpireTime() {
    return expire_time;
}

void DESLayer::setExpireTime(int expireTime) {
    expire_time = expireTime;
}

void DESLayer::printAftEvtQueue() {
    cout << "==>    ";
    list<Event*>::iterator it;
    for (it = eventQueue.begin(); !eventQueue.empty() && it != eventQueue.end(); ++it) {
        printf("%d:%d:%s ", (*it)->getEvtTimestamp(), (*it)->getEvtProcess(), converTransition((*it)->getTransition()).c_str());
    }
    printf("\n");
}

string DESLayer::converTransition(Transition t) {
    string s;
    switch (t) {
        case READY_TO_RUNING:
            s = "RUNNG";
            break;
        case RUNING_TO_BLOCKED:
            s = "BLOCK";
            break;
        case BLOCKED_TO_READY:
            s = "READY";
            break;
        case CREATED_TO_READY:
            s = "READY";
            break;
        case RUNING_TO_READY:
            s = "READY";
            break;
        default:
            break;
    }
    return s;

}




bool DESLayer::empty() {
    return eventQueue.empty();
}

Event* DESLayer::getEvent() {
    return eventQueue.front();
}

void DESLayer::updateProcStates(Transition t, Event* e, Process* p) {
    Transition next;
    switch (t) {
        case CREATED_TO_READY:
            next = READY_TO_RUNING;
            break;
        case READY_TO_RUNING:
            next = RUNING_TO_BLOCKED;
            break;
        case BLOCKED_TO_READY:
            next = READY_TO_RUNING;
            break;
        case RUNING_TO_READY:
            next = READY_TO_RUNING;
        case RUNING_TO_BLOCKED:
            next = BLOCKED_TO_READY;
            break;
        default:
            break;
    }

    e->setProcStates(next, p);

}

int DESLayer::get_next_event_time() {
    if (eventQueue.empty()) {
        int null;
        return null;
    }
    Event* event = eventQueue.front();
    return event->getEvtTimestamp();
}

void DESLayer::removeEvent() {
    eventQueue.erase(eventQueue.begin());
}

Event* DESLayer::getEventById(int id) {
    list<Event*>::iterator it;
    for (it = eventQueue.begin(); !eventQueue.empty() && it != eventQueue.end(); ++it) {
        if ((*it)->getEvtProcess() == id) {
            return *it;
        }
    }
    return nullptr;
}

void DESLayer::remove(Event *e) {
    eventQueue.remove(e);
}
