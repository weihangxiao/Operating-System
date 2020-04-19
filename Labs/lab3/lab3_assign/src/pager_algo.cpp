//
// Created by Weihang Xiao on 4/18/20.
//

#include "pager_algo.h"

FIFO_Pager::FIFO_Pager(int size) : Pager_Algo() {
    counter = 0;
    frame_size = size;
    index_of_frame = 0;
}

frame* FIFO_Pager::select_victim_frame(vector<frame*>& frame_table) {
    index_of_frame = counter % frame_size;
    counter++;
    return frame_table[index_of_frame];
}

int FIFO_Pager::getIndexOfFrame() {
    return index_of_frame;
}
