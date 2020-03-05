//
// Created by Weihang Xiao on 3/4/20.
//

#include "process.h"

Process::Process(int pid, int at, int tc, int cb, int io) {
    PID = pid;
    AT = at;
    TC = tc;
    CB = cb;
    IO = io;
}

void Process::setPID(int pid) {
    PID = pid;
}

void Process::getPID() {
    return PID;
}

void Process::setAT(int at) {
    AT = at;
}

void Process::getAT(){
    return AT;
}

void Process::setCB(int cb) {
    CB = cb;
}

void Process::getCB() {
    return CB;
}

void Process::setTC(int tc) {
    TC = tc;
}

void Process::getTC() {
    return TC;
}

void Process::setIO(int io) {
    IO = io;
}

void Process::getIO() {
    return IO;
}

