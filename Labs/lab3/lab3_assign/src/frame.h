//
// Created by Weihang Xiao on 4/18/20.
//

#ifndef SRC_FRAME_H
#define SRC_FRAME_H


class frame {
    int index_of_vpage;
    bool mapped;

public:
    frame();

    int getIndexOfVpage() const;

    void setIndexOfVpage(int indexOfVpage);

    bool isMapped() const;

    void setMapped(bool mapped);

};


#endif //SRC_FRAME_H
