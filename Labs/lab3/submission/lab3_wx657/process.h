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
    static const int page_table_size = 64;
public:
    Process(int);
    int PID;
    vector<vma> vmas;
    vector<pte_t> page_table;

    unsigned long long unmaps;
    unsigned long long maps;
    unsigned long long ins;
    unsigned long long outs;
    unsigned long long fins;
    unsigned long long fouts;
    unsigned long long zeros;
    unsigned long long segv;
    unsigned long long segprot;
};


#endif //SRC_PROCESS_H
