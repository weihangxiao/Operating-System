//
// Created by Weihang Xiao on 3/4/20.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

//include "DESLayer.h"
using namespace std;

int vflag = 0, tflag = 0, eflag = 0;
char *svalue = NULL;

bool parseCommand(int argc, char* argv[]) {
    int c = 0;
    while ((c = getopt (argc, argv, "vtes:")) != -1) {
        switch (c) {
            case 'v':
                vflag = 1;
                break;
            case 't':
                tflag = 1;
                break;
            case 'e':
                eflag = 1;
                break;
            case 's':
                svalue = optarg;
                break;
            case '?':
                if (optopt == 's')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return false;
            default:
                abort();
        }
    }

    return true;
}

void parseInput(String filename) {
    int AT = 0, TC = 0, CB = 0, IB = 0, pid = 0;
    ifstream infile;
    infile.open(filename);
    string line;
    while (getline(infile, line)) {
        istringstream stream(line);
        stream >> AT >> TC >> CB >> IB;
        Process process(pid, AT, TC, CB, IO);


    }
}


int main(int argc, char* argv[]) {

    int quantum = 10000;
    if (!parseCommand(argc, argv)) {
        exit(0);
    }

    string input = argv[argc - 2];
    string rfile = argv[argc - 1];

    parseInput(input);






    printf ("vflag = %d, tflag = %d, eflag = %d, svalue = %s\n",
            vflag, tflag, eflag, svalue);

}