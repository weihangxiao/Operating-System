#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include "operation.h"
#include <vector>
#include "scheduler.h"
using namespace std;

bool s = false, v = false, q = false, f = false;
char algo = 'i'; //algo
Scheduler* scheduler;
vector<operation> op_list;
bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    int op_len;
    while ((c = getopt (argc, argv, "vqfs:")) != -1) {
        switch (c) {
            case 's':
                algo = optarg[0];
                break;
            case 'v':
                v = true;
                break;
            case 'q':
                q = true;
            case 'f':
                f = true;
            default:
                break;
        }
    }
    return true;
}

void parseInput(string filename) {
   int time = 0;
   int track = 0;
   ifstream infile;
   infile.open(filename);
   string line;
   int id = 0;
   while (getline(infile, line)) {
       if (line[0] == '#') continue;
       operation op;
       istringstream stream(line);
       stream >> time >> track;
       op.arrive_t = time;
       op.end_t = time;
       op.start_t = time;
       op.op_id = id;
       op.track = track;
       op_list.push_back(op);
       id++;
   }
}

void initialize_algorithm() {
    if (algo == 'i') {
        scheduler = new FCFS();
    } else if (algo == 'j') {
        scheduler = new SSTF();
    } else if (algo == 's') {
        scheduler = new LOOK();
    } else if (algo == 'c') {
        scheduler = new CLOOK();
    } else if (algo == 'f') {
        scheduler = new FLOOK();
    }
}



int main(int argc, char* argv[]) {
    string input = argv[argc - 1];
    parseCommand(argc, argv);
    initialize_algorithm();


    parseInput(input);
    int total_time;
    int tot_movement = 0;
    int total_turnaround = 0;
    double avg_turnaround = 0;
    double avg_waittime = 0;
    int max_waittime = 0;
    int total_waittime = 0;
    int wait_time = 0;

    int i = 0;
    int index = 0;
    int head = 0;
    int curr_track = 0;
    bool io_active = false;
    while (true) {
        if (io_active && i == scheduler->getOper().end_t) {
            operation op = scheduler->getOper();
            wait_time = i - op.arrive_t;
            if (v) {
                printf("%d:   %d finish %d\n", i, op.op_id,
                       wait_time);
            }
            curr_track = op.track;
            scheduler->setCurrHead(curr_track);
            scheduler->deleteOper();
            io_active = false;
            scheduler->setIoActive(io_active);
        }

        if (io_active && i != scheduler->getOper().end_t) {
            tot_movement++;
        }

        if (i == op_list[index].arrive_t) {
            scheduler->addOper(op_list[index]);
            if (v) {
                printf("%d:   %d add %d\n", i, op_list[index].op_id,
                        op_list[index].track);
            }
            index++;
        }

        if (!io_active && !scheduler->isEmpty()) {
            operation op = scheduler->getOper();
            int end = i + abs(op.track - curr_track);
            scheduler->setOper(end);
            op_list[op.op_id].end_t = end;
            op_list[op.op_id].start_t = i;
            if (v) {
                printf("%d:   %d issue %d %d\n", i,  op.op_id,
                       op.track, curr_track);
            }
            io_active = true;
            scheduler->setIoActive(io_active);
            if (i != end) {
                tot_movement++;
            }
        }

        if (io_active && i == scheduler->getOper().end_t) {
            continue;
        }

        if (!io_active && scheduler->isEmpty() && index >= op_list.size()) {
            break;
        }
        i++;
    }

    int waittime = 0;
    for (int i = 0; i < op_list.size(); i++) {
        operation op = op_list[i];
        total_turnaround += op.end_t - op.arrive_t;
        waittime = op.start_t - op.arrive_t;
        total_waittime += waittime;
        if (waittime > max_waittime) max_waittime = waittime;
        printf("%5d: %5d %5d %5d\n", op.op_id, op.arrive_t,
               op.start_t, op.end_t);
    }
    avg_turnaround = double(total_turnaround) / op_list.size();
    avg_waittime = double(total_waittime) / op_list.size();
    printf("SUM: %d %d %.2lf %.2lf %d\n",
            i, tot_movement, avg_turnaround, avg_waittime, max_waittime);

    return 0;
}
