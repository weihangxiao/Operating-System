//
// Created by Weihang Xiao on 4/18/20.
//

#include <string>
#include "page_fault_handler.h"

page_fault_handler::page_fault_handler() {
}

bool page_fault_handler::check_valid_page(Process* process, int page_index) {
    vector<vma> vmas = process->vmas;
    for (int i = 0; i < vmas.size(); i++) {
        vma v = vmas[i];
        if (page_index >= v.start_vpage && page_index <= v.end_vpage) {
            process->page_table[page_index].WRITE_PROTECT = v.write_protected;   // set write protected bits
            process->page_table[page_index].FILE_MAPPED = v.file_mapped;  // set file mapped bits
            return true;
        }
    }
    return false;
}

void page_fault_handler::unmap(Process* process, int index_of_page) {
    process->page_table[index_of_page].PRESENT = 0;
}

bool page_fault_handler::isModified(Process *process, int index_of_page) {
    if (process->page_table[index_of_page].MODIFIED == 1) {
        return true;
    }
    return false;
}

bool page_fault_handler::isFileMapped(Process *process, int index_of_page) {
    return process->page_table[index_of_page].FILE_MAPPED == 1;
}

bool page_fault_handler::isPageOut(Process *process, int index_of_page) {
    return process->page_table[index_of_page].PAGEDOUT == 1;
}

void page_fault_handler::unModified(Process *process, int index_of_page) {
    process->page_table[index_of_page].MODIFIED = 0;
}

void page_fault_handler::updateBits(Process *process, int index_of_page, string op) {
    if (op == "w") {
        process->page_table[index_of_page].MODIFIED = 1;
        process->page_table[index_of_page].REFERENCE = 1;
    } else if (op == "r") {
        process-> page_table[index_of_page].REFERENCE = 1;
    }
}

void page_fault_handler::mapping(Process *process, int frame_index, int index_of_page) {
    process->page_table[index_of_page].FRAME_INDEX = frame_index;
    process->page_table[index_of_page].PRESENT = 1;
}