//
// Created by Weihang Xiao on 4/17/20.
//

#ifndef SRC_PROCESS_H
#define SRC_PROCESS_H
#include <vector>
#include "PTE.h"

using namespace std;

struct vma {
    int start_vpage;
    int end_vpage;
    int write_protected;
    int file_mapped;
};

class Process {
private:
    int PID;
    static const int page_table_size = 64;
    pte_t page_table[page_table_size];
public:
    Process(int);


    vector<vma> vmas;
};


#endif //SRC_PROCESS_H
