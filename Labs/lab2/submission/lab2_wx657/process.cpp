//
// Created by Weihang Xiao on 3/4/20.
//

#include "process.h"

Process::Process(int pid, int at, int tc, int cb, int io, State from, State to) {
    PID = pid;
    AT = at;
    TC = tc;
    CB = cb;
    IO = io;
    this->from = from;
    this->to = to;

    static_prio = 0;
    //state = s;
    rem_cb = cb;
    p_cb = cb;
    p_ib = io;
    rem_tc = tc;
    rem_ib = io;
    preempt = false;
}

int Process::getCw() const {
    return cw;
}

void Process::setCw(int cw) {
    Process::cw = cw;
}

int Process::getTotalCb() {
    return total_cb;
}

void Process::setTotalCb(int totalCb) {
    total_cb = totalCb;
}

int Process::getIt() {
    return IT;
}

void Process::setIt(int it) {
    IT = it;
}

int Process::getAt() {
    return AT;
}

void Process::setAt(int at) {
    AT = at;
}

int Process::getFinishTime() {
    return finish_time;
}

void Process::setFinishTime(int finishTime) {
    finish_time = finishTime;
}

//int Process::getQuantum() {
//    return quantum;
//}
//
//void Process::setQuantum(int quantum) {
//    Process::quantum = quantum;
//}

int Process::getWaitTime() const {
    return wait_time;
}

void Process::setWaitTime(int waitTime) {
    wait_time = waitTime;
}

int Process::getCurrTime() {
    return curr_time;
}

bool Process::isPreempt() {
    return preempt;
}

void Process::setPreempt(bool preempt) {
    Process::preempt = preempt;
}

void Process::setCurrTime(int currTime) {
    curr_time = currTime;
}

int Process::getPCb() {
    return p_cb;
}

int Process::getDynamicPrio() {
    return dynamic_prio;
}

void Process::setPCb(int pCb) {
    p_cb = pCb;
}

void Process::setRemTc(int pRem) {
    rem_tc = pRem;
}

void Process::setPIb(int pIb) {
    p_ib = pIb;
}

void Process::setDynamicPrio(int prio) {
        Process::dynamic_prio = prio;
}

int Process::getStaticPrio() {
    return static_prio;
}

void Process::setStaticPrio(int staticPrio) {
    static_prio = staticPrio;
}

int Process::getRemTc() {
    return rem_tc;
}

int Process::getPIb() {
    return p_ib;
}

int Process::getRemIb() {
    return rem_ib;
}



int Process::getTc() {
    return TC;
}

void Process::setPID(int pid) {
    PID = pid;
}

int Process::getCb() {
    return CB;
}

int Process::getIo() {
    return IO;
}

int Process::getPID() {
    return PID;
}

int Process::getRemCb() {
    return rem_cb;
}

void Process::setRemCb(int remCb) {
    rem_cb = remCb;
}

void Process::setStates(State from, State to) {
    this->from = from;
    this->to = to;
}

