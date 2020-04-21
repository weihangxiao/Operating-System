//
// Created by Weihang Xiao on 4/18/20.
//

#include "pager_algo.h"

FIFO_Pager::FIFO_Pager(int size) : Pager_Algo() {
    counter = 0;
    frame_size = size;
    index_of_frame = 0;
}

frame* FIFO_Pager::select_victim_frame(vector<frame*>& frame_table, vector<Process*>& proc_queue) {
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

frame* Random_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
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

frame* Clock_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
    while (true) {
        index_of_frame = counter % frame_size;
        counter++;
        frame* f = frame_table[index_of_frame];
        int index_of_page = f->getIndexOfVpage();
        int pid = f->getPid();
        if (proc_queue[pid]->page_table[index_of_page].REFERENCE == 0) { // unused and oldest
            return frame_table[index_of_frame];
        } else { // referenced bit is 1, reset to 0
            proc_queue[pid]->page_table[index_of_page].REFERENCE = 0;
        }
    }
}

int Clock_Pager::getIndexOfFrame() {
    return index_of_frame;
}

//-------------------------NRU----------------------------------
NRU_Pager::NRU_Pager(int size) : Pager_Algo() {
    frame_size = size;
    counter = 0;
    index_of_frame = 0;
}

frame* NRU_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
    vector<vector<int>> category(4);
    for (int i = 0; i < category.size(); i++) {
        category.push_back(vector<int>());
    }
    int i;
    for (i = 0; i < frame_table.size(); i++) {
        int pid = frame_table[i]->getPid();
        int index_of_page = frame_table[i]->getIndexOfVpage();
        int rbit = proc_queue[pid]->page_table[index_of_page].REFERENCE;
        int mbit = proc_queue[pid]->page_table[index_of_page].MODIFIED;
        if (rbit == 0 && mbit == 0) {
            category[0].push_back(i);
            break;
        } else if (rbit == 0 && mbit == 1) {
            category[1].push_back(i);
        } else if (rbit == 1 && mbit == 0) {
            category[2].push_back(i);
        } else {
            category[3].push_back(i);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (category[i].size() > 0) {
            index_of_frame = category[i][0];
            break;
        }
    }

    counter++;

    if (counter == 50) {
        counter = 0;

    }



}

int NRU_Pager::getIndexOfFrame() {
    return index_of_frame;
}