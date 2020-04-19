#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include "process.h"
#include "frame.h"
#include "pager_algo.h"
#include "page_fault_handler.h"
using namespace std;

//algorithm
char algo;
//Option flag
bool O = false;
bool P = false;
bool F = false;
bool S = false;
bool x = false;
bool y = false;
bool f = false;
bool a = false;
//frame number
int frame_num = 4;

//instruction vector
vector<pair<string, int>> instruct_list;

vector<Process*> proc_queue;

int index_of_frametable = -1;
int num = -1;

Pager_Algo* pager;
//frame *frame_table = new frame[4];
vector<frame*> frame_table;
bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    int op_len;
    while ((c = getopt (argc, argv, "afo:")) != -1) {
        switch (c) {
            case 'a':
                //algo = optarg[0];
                break;
            case 'f':
                //sscanf(optarg, "%d", &frame_num);
                break;
            case 'o':
                cout << "before" << endl;
                op_len = strlen(optarg);
                cout << "after" << endl;
                for (int i = 0; i < op_len; i++) {
                    switch (optarg[i]) {
                        case 'O':
                            O = true; break;
                        case 'P':
                            P = true; break;
                        case 'F':
                            F = true; break;
                        case 'S':
                            S = true; break;
                        case 'x':
                            x = true; break;
                        case 'f':
                            f = true; break;
                        case 'y':
                            y = true; break;
                        case 'a':
                            a = true; break;
                        default:
                            break;
                    }
                }
            default:
                break;
        }
    }
    return true;
}



void parseInput(string filename) {
    int number_process = -1;
    int number_vmas = 0;
    int pid = 0;
    ifstream infile;
    infile.open(filename);
    string line;
    while (getline(infile, line)) {
        if (line[0] == '#') continue;
        istringstream stream(line);
        stream >> number_process;
        if (number_process != -1) {
            break;
        }
    }

    int start_vpage = 0, end_vpage = 0, write_protected = 0, file_mapped = 0;
    for (int i = 0; i < number_process; i++) {
        while (getline(infile, line)) {
            if (line[0] != '#') break;
        }
        Process* process = new Process(pid);
        istringstream stream(line);
        stream >> number_vmas;
        for (int j = 0; j < number_vmas; j++) {
            getline(infile, line);
            if (line[0] == '#') continue;
            istringstream stream(line);
            stream >> start_vpage >> end_vpage >> write_protected >> file_mapped;
            vma v{start_vpage, end_vpage, write_protected, file_mapped};
            process->vmas.push_back(v);
        }
        proc_queue.push_back(process);
        pid++;
    }

    string op;
    int number;
    while (getline(infile, line)) {
        if (line[0] == '#') continue;
        istringstream stream(line);
        stream >> op >> number;
        instruct_list.push_back(make_pair(op, number));
    }

//    for (int i = 0; i < instruct_list.size(); i++) {
//        cout << instruct_list[i].first << " " + std::to_string(instruct_list[i].second) << endl;
//    }
}




frame* allocate_frame_from_free_list() {
    num++;
    index_of_frametable = num;
    return frame_table[index_of_frametable];
}

bool has_free_frame() {
    return num + 1 < frame_num;
}

//frame* get_frame(frame *frame_table[]) {
//    frame* f = pager->select_victim_frame(frame_table);
//    return f;
//}

void print_frame_table() {
    for (int i = 0; i < frame_num; i++) {
        int page_index = frame_table[i]->getIndexOfVpage();
        int pid = frame_table[i]->getPid();
        if (page_index == -1) {
            printf("* ");
        } else {
            printf("%d:%d ", pid, page_index);
        }
    }
    cout << endl;
}

void print_page_table(Process* process) {
    for (int i = 0; i < process->page_table.size(); i++) {
        pte_t p = process->page_table[i];
        if (p.PRESENT) {
            if (p.REFERENCE && p.MODIFIED) {
                printf("%d:RM- ", i);
            } else if (p.REFERENCE) {
                printf("%d:R-- ", i);
            }
        } else {
            printf("* ");
        }
    }
    cout << endl;
}


int main(int argc, char* argv[]) {
    string input = argv[argc - 1];
    string rfile = argv[argc - 1];
    parseCommand(argc, argv);
    parseInput(input);
    //parseRfile(rfile);

    for (int i = 0; i < frame_num; i++) {
        frame_table.push_back(new frame());
    }

    pager = new FIFO_Pager(frame_num);

    unsigned long long counter = 0;
    unsigned long long size = instruct_list.size();
    Process* current_process;
    frame* newframe;
    while (counter < size) {
        string op = instruct_list[counter].first;
        int vpage = instruct_list[counter].second;
        if (O) {
            printf("%llu: ==> %s %d\n", counter, op.c_str(), vpage);
        }
        if (op == "c") {
            current_process = proc_queue[vpage];
            counter++;
            continue;
        }

        int pid = current_process->PID;
        pte_t pte = current_process->page_table[vpage];
        page_fault_handler* handler = new page_fault_handler();
        if (!pte.PRESENT) { //page fault exception
            //search the virtual address in the VMA list,
            // and store bits (file_mapped, write_protect) bits based on VMA
            if (!handler->check_valid_page(current_process, vpage)) {
                counter++;
                continue;
            }

            if (has_free_frame()) {
                newframe = allocate_frame_from_free_list();
            } else {
                newframe = pager->select_victim_frame(frame_table);
                index_of_frametable = pager->getIndexOfFrame();
                int old_page_num = newframe->getIndexOfVpage();
                handler->unmap(current_process,old_page_num);
                if (O) {
                   printf("UNMAP %3d:%3d\n", pid, old_page_num);
                }
                if (handler->isFileMapped(current_process, old_page_num)) {
                    if (O) {
                        printf("FOUT\n");
                    }
                }
                if (handler->isModified(current_process, old_page_num)) {
                    if (O) {
                        printf("OUT\n");
                    }
                }
                handler->unModified(current_process, old_page_num);
            }

            if (handler->isPageOut(current_process, vpage)) {
                if (O) {
                    printf("IN\n");
                }
            } else {
                if (O) {
                    printf("ZERO\n");
                }
            }

            if (O) {
                printf("MAP %3d\n", index_of_frametable);
            }
            handler->mapping(current_process, index_of_frametable, vpage);
            frame_table[index_of_frametable]->setIndexOfVpage(vpage);  // update mapping
            frame_table[index_of_frametable]->setPid(pid);

            if (f) { // print frametable
                print_frame_table();
            }
        }
        handler->updateBits(current_process, vpage, op);


        if (x) {
            print_page_table(current_process);
        }
        counter++;
    }


    return 0;
}
