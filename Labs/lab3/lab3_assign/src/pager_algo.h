//
// Created by Weihang Xiao on 4/18/20.
//

#ifndef SRC_PAGER_ALGO_H
#define SRC_PAGER_ALGO_H

#include "frame.h"
#include <vector>
#include "process.h"
using namespace std;
class Pager_Algo {
public:
    virtual frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue) = 0;
    virtual int getIndexOfFrame() = 0;
    virtual void needReset(bool) = 0;
    virtual void setCurrTime(unsigned long long) = 0;
};

class FIFO_Pager: public Pager_Algo {
private:
    int counter;
    int index_of_frame;
    int frame_size;
public:
    FIFO_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool) {}
    void setCurrTime(unsigned long long) {}
};

class Random_Pager: public Pager_Algo {
private:
    int frame_size;
    vector<int> rand_list;
    int counter;
    int index_of_frame;
public:
    Random_Pager(int, vector<int>);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool) {}
    void setCurrTime(unsigned long long) {}
};

class Clock_Pager: public Pager_Algo {
private:
    int frame_size;
    int counter;
    int index_of_frame;
public:
    Clock_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool) {}
    void setCurrTime(unsigned long long) {}
};

class NRU_Pager: public Pager_Algo {
private:
    int frame_size;
    int counter;
    int index_of_frame;
    bool reset;
public:
    NRU_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool);
    void setCurrTime(unsigned long long) {}
};

struct age_bit {
    unsigned int age: 32;
};

class Aging_Pager: public Pager_Algo {
private:
    int frame_size;
    int counter;
    int index_of_frame;
    vector<age_bit> bit_counter;
public:
    Aging_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool) {}
    void setCurrTime(unsigned long long) {}
};

class Working_Set_Pager: public Pager_Algo {
private:
    int frame_size;
    int counter;
    int index_of_frame;
    unsigned long long curr_time;
public:
    Working_Set_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue);
    int getIndexOfFrame();
    void needReset(bool) {}
    void setCurrTime(unsigned long long);
};



#endif //SRC_PAGER_ALGO_H
