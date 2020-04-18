//
// Created by Weihang Xiao on 4/18/20.
//

#include "frame.h"

frame::frame() {
    index_of_vpage = -1;
    mapped = false;
}


int frame::getIndexOfVpage() const {
    return index_of_vpage;
}

void frame::setIndexOfVpage(int indexOfVpage) {
    index_of_vpage = indexOfVpage;
}

bool frame::isMapped() const {
    return mapped;
}

void frame::setMapped(bool mapped) {
    frame::mapped = mapped;
}
