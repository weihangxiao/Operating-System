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
    reset = false;
}

frame* NRU_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
    vector<vector<int>> category(4, vector<int>());
    int index = counter;

    while (true) {
        int pid = frame_table[index]->getPid();
        int index_of_page = frame_table[index]->getIndexOfVpage();
        int rbit = proc_queue[pid]->page_table[index_of_page].REFERENCE;
        int mbit = proc_queue[pid]->page_table[index_of_page].MODIFIED;
        if (reset) {
           //int present = proc_queue[pid]->page_table[index_of_page].PRESENT;
           //if (present) {
               proc_queue[pid]->page_table[index_of_page].REFERENCE = 0;
           //}
        }

        if (rbit == 0 && mbit == 0) {
            category[0].push_back(index);
            if (!reset) {
                break; // when a frame for class-0 (R=0,M=0) is encountered, you should stop the scan
            }
        } else if (rbit == 0 && mbit == 1) {
            category[1].push_back(index);
        } else if (rbit == 1 && mbit == 0) {
            category[2].push_back(index);
        } else {
            category[3].push_back(index);
        }

        index++;
        index = index % frame_size;
        if (index == counter) {
            break;
        }
    }

    reset = false;
    for (int i = 0; i < 4; i++) {
        if (category[i].size() > 0) {
            index_of_frame = category[i][0];
            break;
        }
    }
    category.clear();
    counter = index_of_frame + 1; // advance to next position of victim frame
    counter = counter % frame_size;
    return frame_table[index_of_frame];
}

int NRU_Pager::getIndexOfFrame() {
    return index_of_frame;
}

void NRU_Pager::needReset(bool reset) {
    this->reset = reset;
}

//---------------------------------Aging----------------------------------
Aging_Pager::Aging_Pager(int size) : Pager_Algo() {
    frame_size = size;
    counter = 0;
    index_of_frame = 0;
}

frame* Aging_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
    for (int i = 0; i < frame_size; i++) {
        //bit_counter[i].age >>= 1;
        age_bit a = frame_table[i]->a;
        unsigned long long age = a.age;
        age >>= 1;
        int pid = frame_table[i]->getPid();
        int index_of_page = frame_table[i]->getIndexOfVpage();
        unsigned int rbit = proc_queue[pid]->page_table[index_of_page].REFERENCE;
        if (rbit == 1) {
            //frame_table[i]->age = frame_table[i]->age | 0x80000000;
            age = age | 0x80000000;
        } else {
            //frame_table[i]->age = frame_table[i]->age | 0;
            age = age | 0;
        }
        frame_table[i]->a.age = age;


    }

    int index = counter;// current position
    unsigned long long min = 0xffffffffffffffff; //max value
    while (true) {
        int pid = frame_table[index]->getPid();
        int index_of_page = frame_table[index]->getIndexOfVpage();
        age_bit a = frame_table[index]->a;
        cout << " ";
        if (a.age < min) {
            min = a.age;
            index_of_frame = index;
        }
        proc_queue[pid]->page_table[index_of_page].REFERENCE = 0;
        index++;
        index = index % frame_size;
        if (index == counter) {
            break;
        }
    }
    frame_table[index_of_frame]->a.age = 0;
    counter = index_of_frame + 1; // advance to next position of victim frame
    counter = counter % frame_size;
    return frame_table[index_of_frame];
}


int Aging_Pager::getIndexOfFrame() {
    return index_of_frame;
}

//---------------------Working-Set-Pager-----------------

Working_Set_Pager::Working_Set_Pager(int size) : Pager_Algo() {
    frame_size = size;
    counter = 0;
    index_of_frame = -1;
    curr_time = 0;
}

frame* Working_Set_Pager::select_victim_frame(vector<frame *> &frame_table, vector<Process*>& proc_queue) {
    int index = counter;
    unsigned long long min = 0xffffffffffffffff; //max value
    int min_index = 0;
    bool foundFrame = false;
    while (true) {
        int pid = frame_table[index]->getPid();
        int index_of_page = frame_table[index]->getIndexOfVpage();
        int last_used_time = frame_table[index]->getLastUsedTime();
        int rbit = proc_queue[pid]->page_table[index_of_page].REFERENCE;
        unsigned long long age = curr_time - last_used_time;
        if (rbit == 1) {
            frame_table[index]->setLastUsedTime(curr_time);
            proc_queue[pid]->page_table[index_of_page].REFERENCE = 0;
        }
        if (rbit == 0 && age > 49){
            index_of_frame = index;
            foundFrame = true;
            break;
        }

        if (frame_table[index]->getLastUsedTime() < min) {
            min = frame_table[index]->getLastUsedTime();
            min_index = index;
        }


//        if (index_of_frame != -1) { // the frame will be selected
//            break;
//        }

        index++;
        index = index % frame_size;
        if (index == counter) {
            break;
        }
    }

    if (!foundFrame) {
        index_of_frame = min_index;
    }
    counter = index_of_frame + 1; // advance to next position of victim frame
    counter = counter % frame_size;
    return frame_table[index_of_frame];

}


int Working_Set_Pager::getIndexOfFrame() {
    return index_of_frame;
}

void Working_Set_Pager::setCurrTime(unsigned long long curr_time) {
        this->curr_time = curr_time;
}


