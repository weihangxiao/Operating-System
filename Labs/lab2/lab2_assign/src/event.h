//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_EVENT_H
#define LAB2_EVENT_H

#include <process.h>

enum Transition {
    CREATED_TO_READY,
    READY_TO_RUNING,
    RUNING_TO_READY,
    RUNING_TO_BLOCKED,
    BLOCKED_TO_READY,
    DONE
};

class Event {
public:
    Event(int, int, Transition);

    int getEvtTimestamp();

    int getEvtProcess();

    Transition getTransition();

    void setTransition(Transition transition);

    void setProcStates(Transition transition1, Process* p);

    void setEvtTimestamp(int evtTimestamp);

    int evtTimestamp;

    //process the event works on
    int evtProcess;
    Transition transition;




};


#endif //LAB2_EVENT_H
