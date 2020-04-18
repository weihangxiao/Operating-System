#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <unistd.h>
#include "process.h"

using namespace std;

//algorithm
string algo;
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

//instruction vector
vector<pair<string, int>> instruct_list;

vector<Process*> proc_queue;


bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    int op_len = 0;
    while ((c = getopt (argc, argv, "aof:")) != -1) {
        switch (c) {
            case 'a':
                algo = optarg[0];
            case 'f':
                sscanf(optarg, "%d", &frame_num);
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



int main(int argc, char* argv[]) {
    string input = argv[argc - 1];
    string rfile = argv[argc - 1];
    parseCommand(argc, argv);
    parseInput(input);
    //parseRfile(rfile);

    int size = instruct_list.size();

    int i = 0;
    while (i < size) {

    }






    return 0;
}
