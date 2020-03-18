//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_DESLAYER_H
#define LAB2_DESLAYER_H

#include <list>
#include <iostream>
#include "event.h"

using namespace std;

class DESLayer {
public:
    int expire_time;
    //constructor
    void addEvent(Event* e);
    bool empty();
    Event* getEvent();
    void updateProcStates(Transition, Event* e, Process* p);
    int get_next_event_time();
    void removeEvent();
    list<Event*> eventQueue;


    void printBefEvtQueue(Event* e);

    string converTransition(Transition t);

    void printAftEvtQueue();

    int getExpireTime();

    void setExpireTime(int expireTime);

    Event* getEventById(int);
    void remove(Event* e);
};


#endif //LAB2_DESLAYER_H
