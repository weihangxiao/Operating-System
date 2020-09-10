#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include "process.h"
#include "frame.h"
#include <queue>
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
int frame_num = 0;

vector<Process*> proc_queue;
vector<pair<string, int>> instruct_list;
queue<int> frame_order;
//instruction vector

vector<int> rand_vals;
int index_of_frametable = -1;
//int num = 0;

Pager_Algo* pager;
vector<frame*> frame_table;

int size = 0; // rand_vals size;

bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    int op_len;
    while ((c = getopt (argc, argv, "f:a:o:")) != -1) {
        switch (c) {
            case 'f':
                sscanf(optarg, "%d", &frame_num);
                break;
            case 'a':
                algo = optarg[0];
                break;
            case 'o':
                op_len = strlen(optarg);
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

void parseRfile(string filename) {
    int r_number;
    string line;
    ifstream infile;
    infile.open(filename);
    getline(infile, line);
    istringstream stream(line);
    stream >> size;
    if (size == 0) {
        size = 1;
        rand_vals.push_back(0);
        infile.close();
        return;
    }
    while (getline(infile, line)) {
        istringstream stream(line);
        stream >> r_number;
        rand_vals.push_back(r_number);
    }
    if (rand_vals.size() == 0) {
        rand_vals.push_back(0);
    }
    infile.close();

}





frame* allocate_frame_from_free_list() {
    //num++;
//    index_of_frametable = num;
    return frame_table[index_of_frametable];
}

bool has_free_frame() {
    while (!frame_order.empty()) {
        index_of_frametable = frame_order.front();
        frame_order.pop();
        return true;
    }

    int num = 0;
    while (num < frame_num) {
        if (frame_table[num]->getIndexOfVpage() == -1 && frame_table[num]->getPid() == -1) {
            index_of_frametable = num;
            return true;
        }
        num++;
    }
    return false;
}

//frame* get_frame(frame *frame_table[]) {
//    frame* f = pager->select_victim_frame(frame_table);
//    return f;
//}

void print_frame_table() {
    printf("FT: ");
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
    printf("PT[%d]: ", process->PID);
    for (int i = 0; i < process->page_table.size(); i++) {
        pte_t p = process->page_table[i];
        if (p.PRESENT) {
            printf("%d:", i);
            if (p.REFERENCE) {
                printf("R");
            } else {
                printf("-");
            }

            if (p.MODIFIED) {
                printf("M");
            } else {
                printf("-");
            }
            if (p.PAGEDOUT) {
                printf("S ");
            } else {
                printf("- ");
            }
        } else if (p.PAGEDOUT){
            printf("# ");
        } else {
            printf("* ");
        }
    }
    cout << endl;
}

void initialize_algorithm() {
    if (algo == 'f') {
        pager = new FIFO_Pager(frame_num);
    } else if (algo == 'r') {
        pager = new Random_Pager(frame_num, rand_vals);
    } else if (algo == 'c') {
        pager = new Clock_Pager(frame_num);
    } else if (algo == 'e') {
        pager = new NRU_Pager(frame_num);
    } else if (algo == 'a') {
        pager = new Aging_Pager(frame_num);
    } else if (algo == 'w') {
        pager = new Working_Set_Pager(frame_num);
    }

}



int main(int argc, char* argv[]) {
    string input = argv[argc - 2];
    string rfile = argv[argc - 1];
    parseCommand(argc, argv);
    parseInput(input);
    parseRfile(rfile);

    for (int i = 0; i < frame_num; i++) {
        frame_table.push_back(new frame());
    }

    initialize_algorithm();

    unsigned long long counter = 0;
    unsigned long long size = instruct_list.size();
    unsigned long long inst_count = 0;
    unsigned long long ctx_switches = 0;
    unsigned long long process_exits = 0;
    unsigned long long cost = 0;

    int number_instr = 0; // for NRU, check if over 50 since last time

    Process* current_process;
    frame* newframe;
    page_fault_handler* handler = new page_fault_handler();
    while (counter < size) {
        string op = instruct_list[counter].first;
        int vpage = instruct_list[counter].second;

        if (O) {
            printf("%llu: ==> %s %d\n", counter, op.c_str(), vpage);
        }
        if (op == "c") {
            current_process = proc_queue[vpage];
            counter++;
            number_instr++;
            ctx_switches++;
            //cost += 121;
            continue;
        }

        int pid = current_process->PID;
        pte_t pte = current_process->page_table[vpage];

        if (op == "e") {
            if (O) {
                printf("EXIT current process %d\n", pid);
            }

            for (int i = 0; i < current_process->page_table.size(); i++) {
                if (current_process->page_table[i].PRESENT) {
                    int frame_index = current_process->page_table[i].FRAME_INDEX;
                    frame_order.push(frame_index);
                    handler->unmap(current_process, i);
                    if (O) {
                        printf(" UNMAP %d:%d\n", pid, i);
                    }
                    //cost += 400;
                    current_process->unmaps++;
                    //used frame has to be return to the free pool
                    frame_table[frame_index]->setIndexOfVpage(-1);
                    frame_table[frame_index]->setPid(-1);

                    if (handler->isModified(current_process, i)) {
                        if (handler->isFileMapped(current_process, i)) {
                            if (O) {
                                printf(" FOUT\n");
                            }
                            //cost += 2500;
                            current_process->fouts++;
                        }
                    }

                    frame_table[frame_index]->a.age = 0;
                }
                current_process->page_table[i].PAGEDOUT = 0;
            }
            //cost += 175;
            process_exits++;
            counter++;
            number_instr++;
            continue;
        }


        cost += 1; // each access (read or write) costs 1 cycles
        if (!handler->check_valid_page(current_process, vpage)) {
            printf(" SEGV\n");
            current_process->segv++;
            number_instr++;
            counter++;
            //cost += 240;
            continue;
        }

        if (!pte.PRESENT) { //page fault exception
            //search the virtual address in the VMA list,
            // and store bits (file_mapped, write_protect) bits based on VMA


            if (has_free_frame()) {
                newframe = allocate_frame_from_free_list();
                frame_table[index_of_frametable]->setLastUsedTime(counter);
            } else {

                if (algo == 'e' && number_instr >= 49) {
                    pager->needReset(true);
                    number_instr = -1;
                }

                if (algo == 'w') {
                    pager->setCurrTime(counter);
                }

                newframe = pager->select_victim_frame(frame_table, proc_queue);
                index_of_frametable = pager->getIndexOfFrame();
                int old_page_num = newframe->getIndexOfVpage();
                int old_pid = newframe->getPid();
                Process* old_process = proc_queue[old_pid];
                handler->unmap(proc_queue[old_pid],old_page_num);
                if (O) {
                   printf(" UNMAP %d:%d\n", old_pid, old_page_num);
                }
                //cost += 400;
                old_process->unmaps++;
                if (handler->isModified(old_process, old_page_num)) {
                    if (handler->isFileMapped(old_process, old_page_num)) {
                        if (O) {
                            printf(" FOUT\n");
                        }
                        //cost += 2500;
                        old_process->fouts++;
                    } else {
                        if (O) {
                            printf(" OUT\n");
                        }
                        //cost += 3000;
                        old_process->outs++;
                        old_process->page_table[old_page_num].PAGEDOUT = 1;
                    }
                }
                //old_process->page_table[old_page_num].PAGEDOUT = 1;
                handler->unModified(old_process, old_page_num);
            }

            if (handler->isFileMapped(current_process, vpage)) {
                if (O) {
                    printf(" FIN\n");
                }
                //cost += 2500;
                current_process->fins++;
            } else if (handler->isPageOut(current_process, vpage)) {
                if (O) {
                    printf(" IN\n");
                }
                //cost += 3000;
                current_process->ins++;
            } else {
                if (O) {
                    printf(" ZERO\n");
                }
                //cost += 150;
                current_process->zeros++;
            }

            if (O) {
                printf(" MAP %d\n", index_of_frametable);
            }
            //cost += 400;
            current_process->maps++;
            handler->mapping(current_process, index_of_frametable, vpage);
            frame_table[index_of_frametable]->setIndexOfVpage(vpage);  // update mapping
            frame_table[index_of_frametable]->setPid(pid);

            if (f) { // print frametable
                print_frame_table();
            }
        }
        //handler->updateBits(current_process, vpage, op);
        if (op == "w") {
            if (current_process->page_table[vpage].WRITE_PROTECT == 1) {
                current_process->page_table[vpage].REFERENCE = 1;
                if (O) {
                    printf(" SEGPROT\n");
                }
                //cost += 300;
                current_process->segprot++;
            } else {
                current_process->page_table[vpage].MODIFIED = 1;
                current_process->page_table[vpage].REFERENCE = 1;
            }
        } else if (op == "r") {
            current_process-> page_table[vpage].REFERENCE = 1;
        }


        if (x) {
            print_page_table(current_process);
        }
        counter++;
        number_instr++;
    }

    if (P) {
        for (int i = 0; i < proc_queue.size(); i++) {
            print_page_table(proc_queue[i]);
        }
    }

    if (F) {
        print_frame_table();
    }

    for (int i = 0; i < proc_queue.size(); i++) {
        Process* current_process = proc_queue[i];
        printf("PROC[%d]: U=%llu M=%llu I=%llu O=%llu FI=%llu FO=%llu Z=%llu SV=%llu SP=%llu\n",
                current_process->PID, current_process->unmaps, current_process->maps,
                current_process->ins, current_process->outs, current_process->fins,
                current_process->fouts, current_process->zeros, current_process->segv,
                current_process->segprot);
        cost += (current_process->unmaps + current_process->maps) * 400 +
                (current_process->ins + current_process->outs) * 3000 +
                (current_process->fins + current_process->fouts) * 2500 +
                current_process->zeros * 150 + current_process->segv * 240 +
                current_process->segprot * 300;
    }

    cost += ctx_switches * 121 + process_exits * 175;

    printf("TOTALCOST %llu %llu %llu %llu\n", size, ctx_switches, process_exits, cost);

    return 0;
}
