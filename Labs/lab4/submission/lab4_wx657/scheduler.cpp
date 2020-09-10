//
// Created by Weihang Xiao on 4/25/20.
//

#include "scheduler.h"


FCFS::FCFS(): Scheduler() {
    io_active = false;
}


void FCFS::addOper(operation &op) {
    list.push(op);
}

operation FCFS::getOper() {
    return list.front();
}

void FCFS::deleteOper(){
    list.pop();
}

bool FCFS::isEmpty() {
    return list.empty();
}

void FCFS::setOper(int end) {
    list.front().end_t = end;
}

bool FCFS::isIoActive()  {
    return io_active;
}

void FCFS::setIoActive(bool ioActive) {
    io_active = ioActive;
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
    if (direction == 1) {
        for (int i = 0; i < queue.size(); i++) {
            operation op = queue[i];
            if (op.track - curr_head < diff && op.track >= curr_head) {
                diff = op.track - curr_head;
                index = i;
            }
        }
    }

    if (diff == INT_MAX) {
        direction = -1;
    } else {
        return queue[index];
    }

    for (int i = 0; i < queue.size(); i++) {
        operation op = queue[i];
        if (curr_head - op.track < diff && curr_head >= op.track) {
            diff = curr_head - op.track;
            index = i;
        }
    }

    if (diff == INT_MAX) {
        direction = 1;
        return this->getOper();
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


//--------------------------CLOOK----------------------------
CLOOK::CLOOK(): Scheduler() {
    curr_head = 0;
    io_active = false;
    direction = 1;
    index = 0;
}

void CLOOK::addOper(operation &op) {
    queue.push_back(op);
}

void CLOOK::setCurrHead(int head) {
    curr_head = head;
}

operation CLOOK::getOper() {
    if (io_active) {
        return queue[index];
    }


    int diff = INT_MAX;  //go up and find the larger track than current head
    for (int i = 0; i < queue.size(); i++) {
        operation op = queue[i];
        if (op.track - curr_head < diff && op.track >= curr_head) {
            diff = op.track - curr_head;
            index = i;
        }
    }

    if (diff == INT_MAX) {  // if no more larger track than current head
        int min_head = INT_MAX;
        for (int i = 0; i < queue.size(); i++) {
            operation op = queue[i];
            if (op.track < min_head) {
                min_head = op.track;
                index = i;
            }
        }
        return queue[index];
    }

    return queue[index];

}

void CLOOK::setOper(int end) {
    queue[index].end_t = end;
}


bool CLOOK::isEmpty() {
    return queue.empty();
}

void CLOOK::deleteOper() {
    queue.erase(queue.begin() + index );
}

bool CLOOK::isIoActive()  {
    return io_active;
}

void CLOOK::setIoActive(bool ioActive) {
    io_active = ioActive;
}

//--------------------------FLOOK----------------------------
FLOOK::FLOOK(): Scheduler() {
    curr_head = 0;
    io_active = false;
    direction = 1;
    index = 0;
}

void FLOOK::addOper(operation &op) {
    add_queue.push_back(op);
}

void FLOOK::setCurrHead(int head) {
    curr_head = head;
}

operation FLOOK::getOper() {
    if (io_active) {
        return active_queue[index];
    }
    if (active_queue.empty()) {
//        printf("add queue size: %d\n", add_queue.size());
//        printf("activate size: %d\n", active_queue.size());
        swap(active_queue, add_queue);
//        printf("dd queue : %d\n", add_queue.size());
//        printf("activate size: %d\n", active_queue.size());
    }

    int diff = INT_MAX;
    if (direction == 1) {
        for (int i = 0; i < active_queue.size(); i++) {
            operation op = active_queue[i];
            if (op.track - curr_head < diff && op.track >= curr_head) {
                diff = op.track - curr_head;
                index = i;
            }
        }
    }

    if (diff == INT_MAX) {
        direction = -1;
    } else {
        return active_queue[index];
    }

    for (int i = 0; i < active_queue.size(); i++) {
        operation op = active_queue[i];
        if (curr_head - op.track < diff && curr_head >= op.track) {
            diff = curr_head - op.track;
            index = i;
        }
    }

    if (diff == INT_MAX) {
        direction = 1;
        return this->getOper();
    }

    return active_queue[index];

}

void FLOOK::setOper(int end) {
    active_queue[index].end_t = end;
}


bool FLOOK::isEmpty() {
    return add_queue.empty() && active_queue.empty();
}

void FLOOK::deleteOper() {
    active_queue.erase(active_queue.begin() + index );
}

bool FLOOK::isIoActive()  {
    return io_active;
}

void FLOOK::setIoActive(bool ioActive) {
    io_active = ioActive;
}







