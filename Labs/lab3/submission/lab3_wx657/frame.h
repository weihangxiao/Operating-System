//
// Created by Weihang Xiao on 4/18/20.
//

#ifndef SRC_FRAME_H
#define SRC_FRAME_H


struct age_bit {
    unsigned long long age: 32;
};

class frame {
    int index_of_vpage;
    bool mapped;
    int pid;
    unsigned long long last_used_time;
public:
    frame();

    int getIndexOfVpage() const;

    void setIndexOfVpage(int indexOfVpage);

    bool isMapped() const;

    void setMapped(bool mapped);

    int getPid() const;

    void setPid(int pid);

    unsigned long long int getLastUsedTime();

    void setLastUsedTime(unsigned long long int lastUsedTime);


    age_bit a;


};


#endif //SRC_FRAME_H
