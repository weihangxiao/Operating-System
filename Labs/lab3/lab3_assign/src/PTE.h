//
// Created by Weihang Xiao on 4/17/20.
//

#ifndef SRC_PTE_H
#define SRC_PTE_H

struct pte_t {
    unsigned int PRESENT: 1;
    unsigned int WRITE_PROTECT: 1;
    unsigned int MODIFIED: 1;
    unsigned int REFERENCE: 1;
    unsigned int PAGEDOUT: 1;
    unsigned int FILE_MAPPED: 1;
    unsigned int FRAME_INDEX: 26;
};

#endif //SRC_PTE_H
