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

void RR_Scheduler::test_preempt(Process *p, int curtime) {

}


PRIO_Scheduler::PRIO_Scheduler(int prio) : Scheduler() {
    max_prio = prio;
}

void PRIO_Scheduler::add_process(Process *p) {
    int prio = p->getDynamicPrio();
//    cout << std::to_string(prio) << endl;
    if (prio == -1) {
        p->setDynamicPrio(p->getStaticPrio() - 1);
        expireQueue[max_prio - prio - 1].push(p);
    } else {
        activeQueue[max_prio - prio - 1].push(p);
//        cout << std::to_string(max_prio - prio - 1) + " " +
//        std::to_string(activeQueue[max_prio - prio - 1].size()) << endl;
    }
    //cout <<std::to_string(max_prio - prio - 1)<< endl;
}

Process* PRIO_Scheduler::get_next_process() {
    int i = 0;
    while (activeQueue[i].empty() && i < max_prio) {
        i++;
    }
    //cout << std::to_string(i) << endl;
    if (i == max_prio) {
        activeQueue->swap(*expireQueue);
//        swithQueue(&activeQueue, &expireQueue);
        if (checkEmpty()) {
            return nullptr;
        }
    }


    int j = 0;
    while (activeQueue[j].empty() && j < max_prio) {
        j++;
    }
    cout << std::to_string(j) << endl;
    Process* p = activeQueue[j].front();
    //cout <<std::to_string(activeQueue[0].size())<< endl;
    activeQueue[j].pop();
    return p;
}

//void PRIO_Scheduler::updateProcQueue(Process *p, int prio) {
//    activeQueue[max_prio - prio - 1].push(p);
//}

void PRIO_Scheduler::swithQueue(queue<Process*> **active, queue<Process*> **expire) {
    queue<Process*>  *temp = *expire;
    *expire = *active;
    *active = temp;
}

//void PRIO_Scheduler::addExpiredQ(Process *p) {
//    activeQueue[max_prio - 1].pop();
//    int prio = p->getDynamicPrio();
//    expireQueue[max_prio - prio - 1].push(p);
//}

void PRIO_Scheduler::test_preempt(Process *p, int curtime) {
//        if (p->getDynamicPrio() == -1) {
//            p->setDynamicPrio(p->getStaticPrio() - 1);
//            addExpiredQ(p);
//            if (checkEmpty()) {
//                swithQueue();
//            }
//        } else {
//            updateProcQueue(p, p->getDynamicPrio() + 1);
//        }
}

bool PRIO_Scheduler::checkEmpty() {
    int i = 0;
    while (i < max_prio) {
        if (!activeQueue[i].empty()) break;
        i++;
    }
    if (i == max_prio) {
        return true;
    }
    return false;
}
