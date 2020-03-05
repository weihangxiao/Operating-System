//
// Created by Weihang Xiao on 3/4/20.
//

#ifndef LAB2_PROCESS_H
#define LAB2_PROCESS_H


class Process {
private:
    int PID; //process ID

    int AT; //Arrival Time

    int TC; //Total CPU Time

    int CB; //CPU Burst

    int IO; //IO Burst

public:
    Process(int, int, int, int, int);

    void setPID(int);
    int getPID();
    void setAT(int);
    int getAT();
    void setTC(int);
    int getTC();
    void setCB(int);
    int getCB();
    void setIO(int);
    int getIO();






};


#endif //LAB2_PROCESS_H
