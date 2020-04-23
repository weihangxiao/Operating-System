//
// Created by Weihang Xiao on 4/18/20.
//

#include "frame.h"

frame::frame() {
    index_of_vpage = -1;
    mapped = false;
    pid = -1; //
    last_used_time = 0;
}


int frame::getIndexOfVpage() const {
    return index_of_vpage;
}

unsigned long long int frame::getLastUsedTime() {
    return last_used_time;
}

void frame::setLastUsedTime(unsigned long long int lastUsedTime) {
    last_used_time = lastUsedTime;
}

void frame::setIndexOfVpage(int indexOfVpage) {
    index_of_vpage = indexOfVpage;
}

bool frame::isMapped() const {
    return mapped;
}

int frame::getPid() const {
    return pid;
}

void frame::setPid(int pid) {
    frame::pid = pid;
}

void frame::setMapped(bool mapped) {
    frame::mapped = mapped;
}
