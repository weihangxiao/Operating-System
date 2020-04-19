//
// Created by Weihang Xiao on 4/18/20.
//

#ifndef SRC_PAGE_FAULT_HANDLER_H
#define SRC_PAGE_FAULT_HANDLER_H

#include "frame.h"
#include "process.h"
#include <string.h>

using namespace std;

class page_fault_handler {
public:
    page_fault_handler();

    bool check_valid_page(Process* process, int page_index);
    void unmap(Process* process, int index_of_page);
    bool isModified(Process* process, int index_of_page);
    bool isFileMapped(Process* process, int index_of_page);
    bool isPageOut(Process* process, int index_of_page);
    void unModified(Process* process, int index_of_page);
    void updateBits(Process* process, int index_of_page, string op);
    void mapping(Process* process, int frame_index, int index_of_page);
};


#endif //SRC_PAGE_FAULT_HANDLER_H
