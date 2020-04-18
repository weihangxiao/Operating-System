//
// Created by Weihang Xiao on 4/17/20.
//

#ifndef SRC_PTE_H
#define SRC_PTE_H

struct pte_t {
    unsigned int PRESENT: 1;
    unsigned int MODIFIED: 1;
    unsigned int REFERENCE: 1;
    unsigned int PAGEDOUT: 1;
    unsigned int INDEX: 28;
};

#endif //SRC_PTE_H
