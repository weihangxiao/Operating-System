//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_SCHEDULER_H
#define LAB2_SCHEDULER_H

#include "process.h"

#include <queue>
#include <stack>
#include <list>

class Scheduler {
public:
    virtual void add_process(Process *p) = 0;
    virtual Process* get_next_process() = 0;
    virtual void test_preempt(Process *p, int curtime) = 0;
private:
};

class FCFS_Scheduler: public Scheduler {
public:
    FCFS_Scheduler();

    void add_process(Process *p);
    Process* get_next_process();
    void test_preempt(Process *p, int curtime)
private:
    queue<Process*> readyQueue;

};

class LCFS_Scheduler: public Scheduler {
public:
    LCFS_Scheduler();

    void add_process(Process *p);
    Process* get_next_process();
    void test_preempt(Process *p, int curtime)
private:
    stack<Process*> readyQueue;
};

class SRTF_Scheduler: public Scheduler {
public:
    SRTF_Scheduler();

    void add_process(Process *p);
    Process* get_next_process();
    void test_preempt(Process *p, int curtime)
private:
    list<Process*> readyQueue;
};

class RR_Scheduler: public Scheduler {
public:
    RR_Scheduler();
    int quantum;

    void add_process(Process *p);
    Process* get_next_process();
    void test_preempt(Process *p, int curtime)
private:
    queue<Process*> readyQueue;
};

class PRIO_Scheduler: public Scheduler {
public:
    PRIO_Scheduler();
    int quantum;

    void add_process(Process *p);
    Process* get_next_process();
    void test_preempt(Process *p, int curtime)
private:
    queue<Process*> readyQueue;
};





#endif //LAB2_SCHEDULER_H
