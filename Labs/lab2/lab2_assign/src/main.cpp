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
string scheduler_name;

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
                break;
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
        scheduler_name = "FCFS";
        scheduler = new FCFS_Scheduler();
    } else if (s_value == "L") {
        scheduler_name = "LCFS";
        scheduler = new LCFS_Scheduler();
    } else if (s_value == "S") {
        scheduler_name = "SRTF";
        scheduler = new SRTF_Scheduler();
    } else if (s_value == "R") {
        scheduler_name = "RR";
        scheduler = new RR_Scheduler();
    } else if (s_value == "P") {
        scheduler_name = "PRIO";
        scheduler = new PRIO_Scheduler(maxprio);
    } else if (s_value == "E") {
        scheduler_name = "PREPRIO";
        scheduler = new E_Scheduler(maxprio);
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
    int io_count; //how many process wait io;
    bool io_status = false;
    int io_begin;
    double total_io_time = 0.0;
    bool proc_running = false;
    bool preempted = false;
    double cpu_wait_time = 0.0;
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
    Process* current_running;
    while ((event = des -> getEvent())) {
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
                    proc->setFinishTime(event->getEvtTimestamp());
                    if (v_flag == 1) {
                        printTrace(DONE, event, proc, 0);
                    }
                    call_scheduler = true;
                    break;
                }

                if (!io_status) {
                    io_status = true;
                    io_begin = curr_time;
                }
                io_count++;
                ib = myRandom(rem_ib, ofs);
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, ib);
                proc->setPIb(ib);
                proc->setIt(proc->getIt() + ib);
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
                current_running = proc;
                if (proc->isPreempt()) {
                    cb = rem_cb;
                } else {
                    cb = myRandom(proc->getCb(), ofs);
                    //cout <<"ofs " + std::to_string(ofs) << endl;
                    ofs++;
                }
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, cb);
                if (quantum <= cb) {
                    if (remain_tc <= quantum) {
                        proc->setRemCb(cb - remain_tc);
                        proc->setRemTc(0);
                        e = new Event(curr_time + remain_tc, pid, RUNING_TO_BLOCKED);
                        proc->setPCb(remain_tc);
                        proc->setTotalCb(proc->getTotalCb() + remain_tc);
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
                        proc->setTotalCb(proc->getTotalCb() + quantum);
                    }
                } else {
                    if (remain_tc <= cb) {
                        cb = remain_tc;
                    }
                    e = new Event(curr_time + cb, pid, RUNING_TO_BLOCKED);

                    e->setProcStates(RUNING_TO_BLOCKED, proc);
                    proc->setPCb(cb);
                    proc->setTotalCb(proc->getTotalCb() + cb);
                    proc->setRemCb(0);
                    proc->setRemTc(remain_tc - cb);
                    des->setExpireTime(curr_time + cb);
                    proc->setPreempt(false);
                }

//                if (s_value == "P") {
//                        proc->setDynamicPrio(proc->getDynamicPrio() - 1);
//                        //scheduler->test_preempt(proc, curr_time);
//                }
                proc->setCurrTime(curr_time);
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
                io_count--;
                if (io_count == 0) {
                    io_status = false;
                    total_io_time += curr_time - io_begin;
                }

                proc->setCurrTime(curr_time);
                proc->setDynamicPrio(proc->getStaticPrio() - 1); //when a process return from I/O reset
                scheduler->add_process(proc);
                call_scheduler = true;
                break;
            case RUNING_TO_READY:
                if (v_flag == 1) printTrace(event->getTransition(), event, proc, proc->getPCb());
                if (s_value == "E" || s_value == "P") proc->setDynamicPrio(proc->getDynamicPrio() - 1);
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
            current_running = nullptr;
            des->setExpireTime(0);
        } else {
            if (s_value == "E" && (curr == BLOCKED_TO_READY || curr == CREATED_TO_READY)) {
                Event* nextEvt = des->getEventById(current_running->getPID());
                if (proc->getDynamicPrio() > current_running->getDynamicPrio() &&
                    nextEvt->getEvtTimestamp() > curr_time) {
                    //cout << "sss " + std::to_string(nextEvt->getEvtTimestamp()) << endl;
                    if (e_flag == 1) {
                        printf("----> PRIO preemption %d by %d ? 1 TS=%d now=%d) --> YES\n", current_running->getPID(),
                               proc->getPID(), nextEvt->getEvtTimestamp(), curr_time);
                    }
                    des->remove(nextEvt);
                    e = new Event(curr_time, current_running->getPID(), RUNING_TO_READY);
                    int actual_cb = curr_time - current_running->getCurrTime();
                    int original_cb = current_running->getRemCb() + current_running->getPCb();
                    current_running->setRemCb(original_cb - actual_cb);
                    current_running->setRemTc(current_running->getRemTc() + current_running->getPCb() - actual_cb);
                    current_running->setWaitTime(curr_time - current_running->getCurrTime());
                    //current_running->setCw(current_running->getCw() + current_running->getWaitTime());
                    current_running->setPCb(actual_cb);
                    current_running->setTotalCb(current_running->getTotalCb() - current_running->getPCb() + actual_cb);
                    current_running->setPreempt(true);
                    if (e_flag == 1) {
                        des->printBefEvtQueue(e);
                        des->addEvent(e);
                        des->printAftEvtQueue();
                    } else {
                        des->addEvent(e);
                    }
                    current_running = nullptr;
                    des->setExpireTime(0);
                    proc_running = false;
                }
            }
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
                cur_running_proc->setCw(cur_running_proc->getCw() + cur_running_proc->getWaitTime());
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


    int last_finish_itme = 0;
    double turn_around = 0.0;
    double cpu_waiting_time = 0.0;
    double cpu_utilization = 0.0;
    cout << scheduler_name;
    if (scheduler_name == "RR" || scheduler_name == "PRIO" || scheduler_name == "PREPRIO") {
        cout << " " + std::to_string(quantum);
    }
    cout << endl;
    for (int i = 0; i < proc_queue.size(); i++) {
        Process* p = proc_queue[i];
        int TT = p->getFinishTime() - p->getAt();
        //int cpu_wait = TT - p->getTotalCb();
        printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
                p->getPID(), p->getAt(), p->getTc(), p->getCb(), p->getIo(), p->getStaticPrio(),
                p->getFinishTime(), TT, p->getIt(), p->getCw());
        if (p->getFinishTime() > last_finish_itme) {
            last_finish_itme = p->getFinishTime();
        }
        turn_around += TT;
        cpu_waiting_time += p->getCw();
        cpu_utilization += p->getTc();
    }
    printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
            last_finish_itme,
            cpu_utilization * 100/ last_finish_itme,
            total_io_time * 100/ last_finish_itme,
            turn_around / proc_queue.size(),
            cpu_waiting_time / proc_queue.size(),
           proc_queue.size()*1.0 * 100/last_finish_itme);


//    printf ("v_flag = %d, t_flag = %d, e_flag = %d, s_value = %s\n",
//            v_flag, t_flag, e_flag, s_value);

}