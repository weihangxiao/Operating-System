//
// Created by Weihang Xiao on 4/18/20.
//

#ifndef SRC_PAGER_ALGO_H
#define SRC_PAGER_ALGO_H

#include "frame.h"
#include <vector>
using namespace std;
class Pager_Algo {
public:
    virtual frame* select_victim_frame(vector<frame*>& frame_table) = 0;
    virtual int getIndexOfFrame() = 0;
};

class FIFO_Pager: public Pager_Algo {
private:
    int counter;
    int index_of_frame;
    int frame_size;
public:
    FIFO_Pager(int);
    frame* select_victim_frame(vector<frame*>& frame_table);
    int getIndexOfFrame();
};


#endif //SRC_PAGER_ALGO_H
