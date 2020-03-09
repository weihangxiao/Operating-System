//
// Created by Weihang Xiao on 3/4/20.
//

#include "scheduler.h"

FCFS_Scheduler::FCFS_Scheduler() : Scheduler() {

}

void FCFS_Scheduler::add_process(Process *p) {
    readyQueue.push(p);
    //cout << std::to_string(p->getAT()) << endl;

}

Process* FCFS_Scheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* p = readyQueue.front();
    readyQueue.pop();
    return p;
}

LCFS_Scheduler::LCFS_Scheduler() : Scheduler() {

}

void LCFS_Scheduler::add_process(Process *p) {
    readyQueue.push(p);
}

Process* LCFS_Scheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* p = readyQueue.top();
    readyQueue.pop();
    return p;
}


SRTF_Scheduler::SRTF_Scheduler() : Scheduler() {

}

void SRTF_Scheduler::add_process(Process *p) {
    list<Process*>::iterator it;
    for (it = readyQueue.begin(); it != readyQueue.end(); it++) {
        if (p->getRemTc() < (*it)->getRemTc()) {
            break;
        }
    }
    readyQueue.insert(it, p);
}

Process* SRTF_Scheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* p = readyQueue.front();
    readyQueue.erase(readyQueue.begin());
    return p;
}

RR_Scheduler::RR_Scheduler() : Scheduler() {

}

void RR_Scheduler::add_process(Process *p) {
    readyQueue.push(p);
}

Process* RR_Scheduler::get_next_process() {
    if (readyQueue.empty()) return nullptr;
    Process* p = readyQueue.front();
    readyQueue.pop();
    return p;
}


