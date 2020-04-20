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


//----------------------Random-----------------------------

Random_Pager::Random_Pager(int size, vector<int> rand_list) : Pager_Algo() {
    frame_size = size;
    this->rand_list = rand_list;
    counter = 0;
    index_of_frame = 0;
}

frame* Random_Pager::select_victim_frame(vector<frame *> &frame_table) {
    index_of_frame = rand_list[counter % rand_list.size()] % frame_size;
    counter++;
    return frame_table[index_of_frame];
}

int Random_Pager::getIndexOfFrame() {
    return index_of_frame;
}

//----------------------Clock-----------------------------
Clock_Pager::Clock_Pager(int size) : Pager_Algo() {
    frame_size = size;
    counter = 0;
    index_of_frame = 0;
}

frame* Clock_Pager::select_victim_frame(vector<frame *> &frame_table) {
    counter++;
    return frame_table[index_of_frame];
}

int Clock_Pager::getIndexOfFrame() {
    return index_of_frame;
}
