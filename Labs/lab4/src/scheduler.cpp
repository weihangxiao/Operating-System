//
// Created by Weihang Xiao on 4/25/20.
//

#include "scheduler.h"


FCFS::FCFS(): Scheduler() {

}


void FCFS::addOper(operation &op) {
    queue.push(op);
}

operation FCFS::getOper() {
    return queue.front();
}

void FCFS::deleteOper(){
    queue.pop();
}

bool FCFS::isEmpty() {
    return queue.empty();
}

void FCFS::setOper(int end) {
    queue.front().end_t = end;
}


//-----------------------SSTF---------------------
SSTF::SSTF(): Scheduler() {
    curr_head = 0;
    io_active = false;
    index = 0;
}

void SSTF::addOper(operation &op) {
    queue.push_back(op);
}

void SSTF::setCurrHead(int head) {
    curr_head = head;
}

operation SSTF::getOper() {
    if (io_active) {
        return queue[index];
    }

    int diff = INT_MAX;
    for (int i = 0; i < queue.size(); i++) {
        operation op = queue[i];
        if (abs(op.track - curr_head) < diff) {
            diff = abs(op.track - curr_head);
            index = i;
        }
    }

    return queue[index];
}

void SSTF::setOper(int end) {
    queue[index].end_t = end;
}


bool SSTF::isEmpty() {
    return queue.empty();
}

void SSTF::deleteOper() {
    queue.erase(queue.begin() + index );
}

bool SSTF::isIoActive()  {
    return io_active;
}

void SSTF::setIoActive(bool ioActive) {
    io_active = ioActive;
}

//--------------------------LOOK----------------------------
LOOK::LOOK(): Scheduler() {
    curr_head = 0;
    io_active = false;
    direction = 1;
    max_track = 0;
    min_track = 0;
    index = 0;
}

void LOOK::addOper(operation &op) {

    queue.push_back(op);
}

void LOOK::setCurrHead(int head) {
    curr_head = head;
}

operation LOOK::getOper() {
    if (io_active) {
        return queue[index];
    }

    int diff = INT_MAX;
    for (int i = 0; i < queue.size(); i++) {
        operation op = queue[i];
        if (direction == 1) {
            if (op.track - curr_head < diff) {
                diff = op.track - curr_head;
                index = i;
            }
        } else {
            if (curr_head - op.track < diff) {
                diff = curr_head - op.track;
                index = i;
            }
        }
    }

    return queue[index];
}

void LOOK::setOper(int end) {
    queue[index].end_t = end;
}


bool LOOK::isEmpty() {
    return queue.empty();
}

void LOOK::deleteOper() {
    queue.erase(queue.begin() + index );
}

bool LOOK::isIoActive()  {
    return io_active;
}

void LOOK::setIoActive(bool ioActive) {
    io_active = ioActive;
}





