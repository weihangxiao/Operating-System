//
// Created by Weihang Xiao on 3/4/20.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include "DESLayer.h"
#include "event.h"
#include "scheduler.h"

using namespace std;

int v_flag = 0, t_flag = 0, e_flag = 0;
string s_value = "F";
vector<int> rand_vals; //random number
vector<Process*> proc_queue;
int number;
int quantum = 10000;
int maxprio = 4;
DESLayer* des = new DESLayer();
Scheduler* scheduler;

bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    while ((c = getopt (argc, argv, "vtes:")) != -1) {
        switch (c) {
            case 'v':
                v_flag = 1;
                break;
            case 't':
                t_flag = 1;
                break;
            case 'e':
                e_flag = 1;
                break;
            case 's':
                //s_value = string(optarg);
                if (!(optarg && !optarg[0])) {
                    s_value = optarg[0];
                    optarg++;
                    sscanf(optarg, "%d:%d", &quantum, &maxprio);
                }
                break;
            case '?':
                if (optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return false;
            default:
                abort();
        }
    }

    return true;
}

void parseInput(string filename) {
    int AT = 0, TC = 0, CB = 0, IO = 0, pid = 0;
    ifstream infile;
    infile.open(filename);
    string line;
    while (getline(infile, line)) {
        istringstream stream(line);
        stream >> AT >> TC >> CB >> IO;
        Process* process = new Process(pid, AT, TC, CB, IO, CREATED, READY);
        proc_queue.push_back(process);
        Event* event = new Event(AT, pid, CREATED_TO_READY);
        des->addEvent(event);
        pid++;
    }

//    list<Event>::iterator it;
//    for (it = des->eventQueue->begin(); it != des->eventQueue->end(); it++) {
//       cout << std::to_string(it->getEvtTimestamp()) << endl;
//    }
}

void parseRfile(string filename) {
    int r_number;
    string line;
    ifstream infile;
    infile.open(filename);
    getline(infile, line);
    istringstream stream(line);
    stream >> number;
    while (getline(infile, line)) {
        istringstream stream(line);
        stream >> r_number;
        rand_vals.push_back(r_number);
    }
    infile.close();

}


int myRandom(int burst, int ofs) {
    return 1 + (rand_vals[ofs] % burst);
}

void printTrace(Transition t, Event* e, Process* p, int burst) {
    switch (t) {
        case CREATED_TO_READY:
            printf("%d %d %d: CREATED -> READY\n", e->getEvtTimestamp(),
                    p->getPID(), burst);
            break;
        case RUNING_TO_BLOCKED:
            printf("%d %d %d: RUNING -> BLOCK  ib=%d rem=%d\n", e->getEvtTimestamp(),
                    e->getEvtProcess(), p->getPCb(), burst, p->getRemTc());
            break;
        case READY_TO_RUNING:
            printf("%d %d %d: READY -> RUNING  cb=%d rem=%d prio=%d\n", e->getEvtTimestamp(),
                   e->getEvtProcess(), p->getWaitTime(), burst, p->getRemTc(), p->getDynamicPrio());
            break;
        case BLOCKED_TO_READY:
            printf("%d %d %d: BLOCK -> READY\n", e->getEvtTimestamp(),
                   e->getEvtProcess(), burst);
            break;
        case RUNING_TO_READY:
            printf("%d %d %d: RUNING -> READY  cb=%d rem=%d prio=%d\n", e->getEvtTimestamp(),
                   e->getEvtProcess(), p->getPCb(), p->getRemCb(), p->getRemTc(), p->getDynamicPrio());
            break;
        case DONE:
            printf("%d %d %d: Done\n", e->getEvtTimestamp(),
                   e->getEvtProcess(), p->getPCb());
        default:
            break;
    }

}

void initScheduler() {
    if (s_value == "F") {
        scheduler = new FCFS_Scheduler();
    } else if (s_value == "L") {
        scheduler = new LCFS_Scheduler();
    } else if (s_value == "S") {
        scheduler = new SRTF_Scheduler();
    } else if (s_value == "R") {
        scheduler = new RR_Scheduler();
    } else if (s_value == "P") {
        scheduler = new PRIO_Scheduler();
    }

}

//void print(Event &e) {
//    cout << std::to_string(e->getEvtProcess());
//}



int main(int argc, char* argv[]) {
    int ofs = 0;
    int ib;
    int cb;
    bool call_scheduler = false;
    int curr_time;
    bool proc_running = false;
    bool preempted = false;
    if (!parseCommand(argc, argv)) {
        exit(0);
    }
//    printf("%d, %d\n", quantum, maxprio);
    string input = argv[argc - 2];
    string rfile = argv[argc - 1];

    parseInput(input);
    parseRfile(rfile);

    for (int i = 0; i < proc_queue.size(); i++) {
        Process* p = proc_queue.at(i);
        p->setStaticPrio(myRandom(maxprio, ofs));
        p->setDynamicPrio(p->getStaticPrio() - 1);
        ofs++;
    }

    initScheduler();
    Event* event;

    while (event = des -> getEvent()) {
        //Event* event =  (des->getEvent());
        Process* proc = proc_queue.at(event->getEvtProcess());
        //cout << std::to_string(event->getEvtTimestamp()) << endl;
        curr_time = event->getEvtTimestamp();
        int rem_tc = proc->getRemTc();
        int pid = event->getEvtProcess();
        int rem_cb = proc->getRemCb();
        int rem_ib = proc->getRemIb();
        int remain_tc = proc->getRemTc();
        //int time_prev_state = curr_time - proc
        des->removeEvent();
        Event* e;
        Transition curr = event->getTransition();
        switch (curr) {
            case CREATED_TO_READY:
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, 0);
                proc->setCurrTime(curr_time);
                scheduler->add_process(proc);
                call_scheduler = true;
                break;
            case RUNING_TO_BLOCKED:
                if (proc->getRemTc() == 0) {
                    if (v_flag == 1) {
                        printTrace(DONE, event, proc, 0);
                        call_scheduler = true;
                        break;
                    }
                }

                ib = myRandom(rem_ib, ofs);
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, ib);
                proc->setPIb(ib);
                e = new Event(curr_time + ib, pid, BLOCKED_TO_READY);
                if (e_flag == 1) {
                    des->printBefEvtQueue(e);
                    des->addEvent(e);
                    des->printAftEvtQueue();
                } else {
                    des->addEvent(e);
                }
                e->setProcStates(BLOCKED_TO_READY, proc);
                call_scheduler = true;
                ofs++;
                break;
            case READY_TO_RUNING:
                proc_running = true;

                if (proc->isPreempt()) {
                    cb = rem_cb;
                } else {
                    cb = myRandom(proc->getCb(), ofs);
//                    cout << std::to_string(ofs) << endl;
                    ofs++;
                }
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, cb);
                if (quantum <= cb) {
                    if (remain_tc <= quantum) {
                        proc->setRemCb(cb - remain_tc);
                        proc->setRemTc(0);
                        e = new Event(curr_time + remain_tc, pid, RUNING_TO_BLOCKED);
                        proc->setPCb(cb);
                        des->setExpireTime(curr_time + remain_tc);
                        proc->setPreempt(false);
                    } else {   // remain_tc > quantum
                        proc->setRemCb(cb - quantum);
                        if (cb - quantum == 0) {
                            e = new Event(curr_time + quantum, pid, RUNING_TO_BLOCKED);
                            preempted = false;
                            proc->setPreempt(false);
                        } else {   // cb  > quantum
                            e = new Event(curr_time + quantum, pid, RUNING_TO_READY);
                            proc->setPreempt(true);
                        }
                        des->setExpireTime(curr_time + quantum);
                        proc->setRemTc(remain_tc - quantum);
                        proc->setPCb(quantum);
                    }
                } else {
                    if (remain_tc <= cb) {
                        cb = remain_tc;
                    }
                    e = new Event(curr_time + cb, pid, RUNING_TO_BLOCKED);
                    e->setProcStates(RUNING_TO_BLOCKED, proc);
                    proc->setPCb(cb);
                    proc->setRemCb(rem_cb - cb);
                    proc->setRemTc(remain_tc - cb);
                    des->setExpireTime(curr_time + cb);
                    proc->setPreempt(false);
                }


               //proc->setRemCb(rem_cb - cb);
                if (e_flag == 1) {
                    des->printBefEvtQueue(e);
                    des->addEvent(e);
                    des->printAftEvtQueue();
                } else {
                    des->addEvent(e);
                }
//                cout << std::to_string(proc.getPRem()) << endl;
                break;
            case BLOCKED_TO_READY:
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, proc->getPIb());
                proc->setCurrTime(curr_time);
                scheduler->add_process(proc);
                call_scheduler = true;
                break;
            case RUNING_TO_READY:
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, proc->getPCb());
                proc->setCurrTime(curr_time);
                scheduler->add_process(proc);
                call_scheduler = true;
                break;
            case DONE:
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, proc->getPCb());
                break;
            default:
                break;
        }
        delete event;
        event = nullptr;
        //if the current process is still running
        if (curr_time >= des->getExpireTime()) {
            proc_running = false;
            des->setExpireTime(0);
        }

        if (call_scheduler) {
            if (des->get_next_event_time() == curr_time) {
                continue;
            }
            call_scheduler = false;
            if (!proc_running) {
                Process* cur_running_proc = scheduler->get_next_process();
                if (cur_running_proc == nullptr) {
                    continue;
                }

                e = new Event(curr_time, cur_running_proc->getPID(), READY_TO_RUNING);
                cur_running_proc->setWaitTime(curr_time - cur_running_proc->getCurrTime());
                if (e_flag == 1) {
                    des->printBefEvtQueue(e);
                    des->addEvent(e);
                    des->printAftEvtQueue();
                } else {
                    des->addEvent(e);
                }
                e->setProcStates(READY_TO_RUNING, cur_running_proc);

            }
        }
    }




//    printf ("v_flag = %d, t_flag = %d, e_flag = %d, s_value = %s\n",
//            v_flag, t_flag, e_flag, s_value);

}