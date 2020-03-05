//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_EVENT_H
#define LAB2_EVENT_H


class event {
    public:

    int evtTimestamp;

    //process the event works on
    int evtProcess;

    enum Transition {
        TRANS_TO_READY,
        TRANS_TO_RUN,
        TRANS_TO_BLOCK,
        TRANS_TO_PREEMPT
    };

    Transition transition;


};


#endif //LAB2_EVENT_H
