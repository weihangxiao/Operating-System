//
// Created by Weihang Xiao on 3/4/20.
//

#include "event.h"


Event::Event(int ts, int pid, Transition t) {
        evtTimestamp = ts;
        evtProcess = pid;
        transition = t;
}

int Event::getEvtTimestamp(){
    return evtTimestamp;
}

void Event::setTransition(Transition transition) {
    Event::transition = transition;
}


void Event::setProcStates(Transition transition1, Process* p) {
    switch (transition1) {
        case CREATED_TO_READY:
            p->setStates(CREATED, READY);
            break;
        case READY_TO_RUNING:
            p->setStates(READY, RUNING);
            break;
        case RUNING_TO_BLOCKED:
            p->setStates(RUNING, BLOCKED);
            break;
        case BLOCKED_TO_READY:
            p->setStates(BLOCKED, READY);
            break;
        case RUNING_TO_READY:
            p->setStates(RUNING, READY);
            break;
        default:
            break;
    }
}
int Event::getEvtProcess(){
    return evtProcess;
}

Transition Event::getTransition() {
    return transition;
}

void Event::setEvtTimestamp(int evtTimestamp) {
    Event::evtTimestamp = evtTimestamp;
}
