#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <utility>
#include<sstream>
using namespace std;


const string NO_VALUE = "";

ifstream infile;
istringstream is;

string line;
string lastline;

int module_num = 1;
int base_address = 0;
int linenum;
int length;
int offset;

bool complete = false;
bool duplicate = false;
bool definedbefore;
bool newline = false;

vector<pair<string, int> > symboltable;
vector<int> memoryaddress;
vector<string> uselist;
vector<int> module;


struct Tokens {
    int linecount;
    int offset;
    string str;
};


string intToString(int num) {
    stringstream ss;
    ss << num;
    string s;
    ss >> s;
    return s;
}


Tokens getToken() {
    char * token;
    Tokens t;
    if (newline) {
        lastline = line;
        getline(infile, line);
        is.str(line);
        offset = 1;
        linenum++;
        if (infile.eof() && line.empty()) {  //current line is the empty and eof
            int len = lastline.length();
            Tokens t;
            t.linecount = linenum - 1;
            t.offset = len + 1;

            if (complete) {
                t.str = "EOF";
            } else {
                t.str = NO_VALUE;
            }
            return t;
        }
        token = strtok(&line[0], " \t\n");

        if (token == NULL && !infile.eof()) {
            newline = true;
            return getToken();
        } else if (token == NULL && infile.eof()){
            int len = line.length();
            t.linecount = linenum;
            t.offset = len;
            if (complete) {
                t.str = "EOF";
            } else {
                t.str = NO_VALUE;
            }
            return t;
        }

        newline = false;

    } else {
        token = strtok(NULL, " \t\n");

        if (token == NULL && !infile.eof()) {
            newline = true;
            return getToken();
        } else if (token == NULL && infile.eof()){
            int len = line.length();
            t.linecount = linenum;
            t.offset = len;

            if (complete) {
                t.str = "EOF";
            } else {
                t.str = NO_VALUE;
            }
            return t;
        }
    }

    string str = is.str();
    while (line.length() != 0 && (str[offset - 1] == ' ' || str[offset - 1] == '\t')) {
        offset++;
    }

    //offset = line.find(string(token), offset + 1) + 1;
    t.linecount = linenum;
    t.offset = offset;
    t.str = string(token);
    offset = offset + string(token).length();
    return t;
}



void parseerror(int linenum, int lineoffset, int errcode) {
    static string errstr[] = {
            "NUM_EXPECTED",
            "SYM_EXPECTED",
            "ADDR_EXPECTED",
            "SYM_TOO_LONG",
            "TOO_MANY_DEF_IN_MODULE",
            "TOO_MANY_USE_IN_MODULE",
            "TOO_MANY_INSTR"
    };
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode].c_str());
}




Tokens readInt() {
    Tokens token = getToken();

    if (token.str == "EOF") {
        return token;
    }

    if (token.str == NO_VALUE) {
        parseerror(token.linecount, token.offset, 0);  //num expected
        exit(0);
    }

    for (int i = 0; i < token.str.size(); i++) {
        if(token.str[i] < '0' || token.str[i] > '9') {
            parseerror(token.linecount, token.offset, 0); //num expected
            exit(0);
        }
    }

    return token;
}

string readSym() {
    Tokens token = getToken();
    string str = token.str;
    if (!isalpha(str[0])) {
        parseerror(token.linecount, token.offset, 1); //expected symbol
        exit(0);
    }

    for (int i = 0; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            parseerror(token.linecount, token.offset, 1); //expected symbol
            exit(0);
        }
    }

    if (str.length() > 16) {
        parseerror(token.linecount, token.offset, 3); // symbol name is too long
        exit(0);
    }

    return str;
}

int readInstrco() {
    Tokens token = getToken();
    int count = stoi(token.str);
    if (token.str == NO_VALUE) {
        parseerror(token.linecount, token.offset, 0);  //num expected
        exit(0);
    }
    if (count + base_address > 512) {
        parseerror(token.linecount, token.offset, 6); // total num_intr exceed memory size
        exit(0);
    }
    return count;
}


string readIEAR() {
    Tokens token = getToken();
    string str = token.str;
    if (str.length() > 1 || str.length() == 0) {
        parseerror(token.linecount, token.offset, 2); //Addressing expected A/E/I/R
        exit(0);
    }
    char ins = str[0];
    if (ins != 'I' && ins != 'E' && ins != 'R' && ins != 'A') {
        parseerror(token.linecount, token.offset, 2); //Addressing expected A/E/I/R
        exit(0);
    }

    return string(str);
}

void checkInst() {
    Tokens token = getToken();
    for (int i = 0; i < token.str.size(); i++) {
        if (token.str[i] < '0' || token.str[i] > '9') {
            parseerror(token.linecount, token.offset, 0); //num expected
            exit(0);
        }
    }
}

void pass1(string filename) {
    infile.open(filename);
    if (infile.peek() == std::ifstream::traits_type::eof()) { //check empty file
        return;
    }
    while (!infile.eof()) {
        /************ define list ***********/
        Tokens deftoken = readInt();
        if (deftoken.str == "EOF") {
            break;
        }
        int defcount = stoi(deftoken.str);
        if (defcount > 16) {
            parseerror(deftoken.linecount, deftoken.offset, 4); // too many def in module
            exit(0);
        }
        for (int i = 0; i < defcount; i++) {
            string sym = readSym();
            Tokens valtoken = readInt();
            int relativeaddress = stoi(valtoken.str);


            symboltable.push_back(make_pair(sym, relativeaddress + base_address));
            module.push_back(module_num);

        }

        /************ use list ***********/

        Tokens usetoken = readInt();
        int usecount = stoi(usetoken.str);
        if (usecount > 16) {
            parseerror(usetoken.linecount, usetoken.offset, 5); // too many use in module
            exit(0);
        }
        for (int i = 0; i < usecount; i++) {
            string sym = readSym();
        }

        /************ inst list ***********/

        int instcount = readInstrco();
        for (int i = 0; i < instcount; i++) {
            string addressmode = readIEAR();
            checkInst();
        }

        for (int i = 0; i < symboltable.size(); i++) {
            if (module_num == module[i]) {
                int relativeaddress = symboltable[i].second - base_address;
                if (relativeaddress > instcount) {
                    cout << "Warning: Module " << intToString(module_num) << ": " << symboltable[i].first + " too big " + intToString(relativeaddress)
                             << " (max=" + intToString(instcount - 1) + ") " << "assume zero relative" << endl;
                    symboltable[i].second = 0 + base_address;
                }

            }
        }


        module_num++;
        base_address += instcount;
        complete = true;
    }
    infile.close();



}

string getCount(int count) {
    if (count < 10) {
        return "00" + intToString(count);
    } else if (count < 100) {
        return "0" + intToString(count);
    } else {
        return intToString(count);
    }

}

string convertInst(int inst) {
    if (inst < 10) {
        return "000" + intToString(inst);
    } else if (inst < 100) {
        return "00" + intToString(inst);
    } else if (inst < 1000) {
        return "0" + intToString(inst);
    }
    return intToString(inst);
}




void pass2(string filename) {
    infile.open(filename);
    int count = 0;
    module_num = 1;
    base_address = 0;
    cout << "Memory Map" << endl;

    while (!infile.eof()) {
        Tokens deftoken = readInt();
        if (deftoken.str == "EOF") {
            break;
        }
        int defcount = stoi(deftoken.str);
        for (int i = 0; i < defcount; i++) {
            string sym = readSym();
            Tokens valtoken = readInt();

        }

        /************ use list ***********/

        Tokens usetoken = readInt();
        int usecount = stoi(usetoken.str);
        vector<pair<string, bool> > currUselist;
        for (int i = 0; i < usecount; i++) {
            string sym = readSym();
            currUselist.push_back(make_pair(sym, false));
            uselist.push_back(sym);
        }

        /************ inst list ***********/

        int instcount = readInstrco();
        for (int i = 0; i < instcount; i++) {
            string addressmode = readIEAR();
            Tokens token = getToken();
            int instruction = stoi(token.str);
            int opcode = instruction / 1000;
            int operand = instruction - opcode * 1000;
            cout << getCount(count) << ": ";
            if (addressmode == "A") {
                if (instruction >= 10000) {
                    instruction = 9999;
                    cout << convertInst(instruction) << " Error: Illegal opcode; treated as 9999" << endl;
                } else if (operand >= 512) {
                    cout << convertInst(opcode * 1000) << " Error: Absolute address exceeds machine size; zero used" << endl;
                } else {
                    cout << convertInst(instruction) << endl;
                }
            } else if (addressmode == "I") {
                if (instruction >= 10000) {
                    instruction = 9999;
                    cout << convertInst(instruction) << " Error: Illegal immediate value; treated as 9999" << endl;
                } else {
                    cout << convertInst(instruction) << endl;
                }

            } else if (addressmode == "R") {
                if (instruction >= 10000) {
                    instruction = 9999;
                    cout << convertInst(instruction) << " Error: Illegal opcode; treated as 9999" << endl;
                } else if (operand >= instcount) {
                    cout << convertInst(opcode * 1000 + base_address) << " Error: Relative address exceeds module size; zero used" << endl;
                } else {
                    cout << convertInst(instruction + base_address) << endl;
                }
            } else if (addressmode == "E") {
                if (instruction >= 10000) {
                    instruction = 9999;
                    cout << convertInst(instruction) << " Error: Illegal opcode; treated as 9999" << endl;
                } else if (operand > usecount - 1) {
                    cout << convertInst(instruction) << " Error: External address exceeds length of uselist; treated as immediate" << endl;
                } else {
                    bool usedNotDefine = true;
                    currUselist[operand].second = true;
                    for (int i = 0; i < symboltable.size(); i++) {
                        if (symboltable[i].first == currUselist[operand].first) {
                            operand = symboltable[i].second;
                            usedNotDefine = false;
                            break;
                        }
                    }
                    if (usedNotDefine) {
                        cout << convertInst(opcode * 1000) << " Error: "<< currUselist[operand].first  << " is not defined; zero used" << endl;
                    } else {
                        cout << convertInst(opcode * 1000 + operand) << endl;
                    }

                }
            }

            count++;
        }

        for (int i = 0; i < currUselist.size(); i++) {
            if (!currUselist[i].second) {
                cout << "Warning: Module " + intToString(module_num) + ": " + currUselist[i].first + " " <<
                     "appeared in the uselist but was not actually used" << endl;
            }
        }
        module_num++;
        base_address += instcount;
        currUselist.clear();

    }
    infile.close();
    return;
}

void printSymNotUsed() {
    cout << endl;
    for (int i = 0; i < symboltable.size(); i++) {
        bool defineNotUsed = true;
        string sym = symboltable[i].first;
        for (int j = 0; j < uselist.size(); j++) {
            if (uselist[j] == sym) {
                defineNotUsed = false;
                break;
            }
        }
        if (defineNotUsed) {
            cout <<  "Warning: Module " + intToString(module[i]) + ": " + sym + " was defined but never used" << endl;
        }
    }
}




void printsymboltable() {
    map<string, int> occurance;
    for (int i = 0; i < symboltable.size(); i++) {
        string sym = symboltable[i].first;

        occurance[sym]++;

    }

    cout << "Symbol Table" << endl;
    for (int i = 0; i < symboltable.size(); i++) {
        string sym = symboltable[i].first;

        if (occurance.at(sym) == 1) {
            cout << sym + "=" + intToString(symboltable[i].second) << endl;
        } else if (occurance.at(sym) > 1) {
            cout << sym + "=" + intToString(symboltable[i].second)+ " " <<
                 "Error: This variable is multiple times defined; first value used" << endl;
            occurance[sym] = -1;
        }

    }
}


void parser(string filename) {
    pass1(filename);
    printsymboltable();
    cout << endl;
    pass2(filename);
    printSymNotUsed();
}




int main(int argc, char* argv[]) {
    string fn = argv[1]; //filename
    parser(fn);
//    ofstream myfile;
//    myfile.open ("t2.txt");
//    myfile << "2\n\n   x1 \n\t 3\t";
//    myfile.close();
    return 0;
}