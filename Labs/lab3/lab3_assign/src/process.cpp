//
// Created by Weihang Xiao on 4/17/20.
//

#include "process.h"

Process::Process(int PID) {
    this->PID = PID;
    for (int i = 0; i < page_table_size; i++) {
        page_table.push_back(pte_t());
    }
}

