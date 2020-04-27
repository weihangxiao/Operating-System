//
// Created by Weihang Xiao on 4/25/20.
//

#ifndef SRC_SCHEDULER_H
#define SRC_SCHEDULER_H
#include <queue>
#include <vector>
#include "operation.h"
using namespace std;
class Scheduler {
public:

    virtual void addOper(operation &op) = 0;
    virtual operation getOper() = 0;
    virtual  void deleteOper() = 0;
    virtual bool isEmpty() = 0;
    virtual void setOper(int) = 0;
    virtual void setCurrHead(int) = 0;
    virtual bool isIoActive() = 0;
    virtual void setIoActive(bool) = 0;
};

class FCFS: public Scheduler {
public:
    FCFS();
    void addOper(operation &op);
    operation getOper();
    void deleteOper();
    bool isEmpty();
    void setOper(int);
    void setCurrHead(int){}
    virtual bool isIoActive(){}
    virtual void setIoActive(bool){}

    queue<operation> queue;

};

class SSTF: public Scheduler {
private:
    int curr_head;
    int index;
    bool io_active;
public:
    SSTF();
    void addOper(operation &op);
    operation getOper();
    void deleteOper();
    bool isEmpty();
    void setOper(int);
    void setCurrHead(int);
    bool isIoActive();
    void setIoActive(bool ioActive);

    vector<operation> queue;
};

class LOOK: public Scheduler {
private:
    int curr_head;
    int index;
    int direction;
    int max_track;
    int min_track;
    bool io_active;
public:
    LOOK();
    void addOper(operation &op);
    operation getOper();
    void deleteOper();
    bool isEmpty();
    void setOper(int);
    void setCurrHead(int);
    bool isIoActive();
    void setIoActive(bool ioActive);

    vector<operation> queue;
};

#endif //SRC_SCHEDULER_H
