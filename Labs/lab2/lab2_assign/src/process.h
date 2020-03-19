//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H

enum State {
    CREATED,
    READY,
    RUNING,
    BLOCKED,
    PREEMPT
};

class Process {
private:
    int PID; //process ID

    int AT; //Arrival Time

    int TC; //Total CPU Time

    int CB; //CPU Burst

    int IO; //IO Burst

    State from;
    State to;

    int p_cb;

    int rem_tc;

    int p_ib;

    int cw; //cpu_waiting_time

    int static_prio;
    int dynamic_prio;
    int wait_time;
    int rem_cb;
    int rem_ib;
    int curr_time;
    int total_cb;
    //int quantum;
    bool preempt;
    int IT; //I/O time
    int finish_time;
public:
    Process();
    Process(int, int, int, int, int, State, State);

    void setPID(int);
    int getPID();

    void setStates(State, State);

    int getPCb();

    int getRemTc();

    int getPIb();

    int getPrio();

    void setPCb(int pCb);

    void setRemTc(int pRem);

    void setPIb(int pIb);

    void setPrio(int prio);

    int getRemCb();

    void setRemCb(int remCb);

    int getCb();

    int getIo();

    int getTc();

    int getRemIb();

    int getCurrTime();

    void setCurrTime(int currTime);

    int getWaitTime() const;

    void setWaitTime(int waitTime);

    bool isPreempt();

    void setPreempt(bool preempt);

    int getStaticPrio();

    void setStaticPrio(int staticPrio);

    void setDynamicPrio(int prio);

    int getDynamicPrio();

    int getFinishTime();

    void setFinishTime(int finishTime);

    int getAt();

    void setAt(int at);

    int getIt();

    void setIt(int it);

    int getTotalCb();

    void setTotalCb(int totalCb);

    int getCw() const;

    void setCw(int cw);

//    int getQuantum();
//
//    void setQuantum(int quantum);


};


#endif //LAB2_PROCESS_H
