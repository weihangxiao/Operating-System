#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
using namespace std;


//unordered_map<string, int> symtable;



const string NO_VALUE = "";

bool newline = false;
string line;
int linenum = 0;
int length;
int offset = -1;
string lastline;
ifstream infile;
int module_num = 1;
int base_address = 0;
bool complete = false;
map<string, int> symboltable;


struct Tokens {
    int linecount;
    int offset;
    string str;
};



Tokens getToken() {
    char * token;
    Tokens t;
    if (newline) {
        lastline = line;
        getline(infile, line);
        offset = -1;
        linenum++;
        if (infile.eof()) {
            int len = lastline.length();
            Tokens t;
            t.linecount = linenum - 1;
            t.offset = len + 1;

            if (complete) {
                t.str = "complete";
            } else {
                t.str = NO_VALUE;
            }

            return t;
        }
        token = strtok(&line[0], " ");
        if (token == NULL) {
            newline = true;
            return getToken();
        } else {
            newline = false;
        }
    } else {
        token = strtok(NULL, " ");
        //cout << string(token) << endl;
        if (token == NULL) {
            newline = true;
            return getToken();
        }
    }

    offset = line.find(string(token), offset + 1) + 1;
    t.linecount = linenum;
    t.offset = offset;
    t.str = string(token);
    return t;
}



void parseerror(int linenum, int lineoffset, int errcode) {
    static string errstr[] = {
            "NUM_EXPECTED",
            "SYM_EXPECTED",
            "ADDR_EXPECTED",
            "SYM_TOLONG",
            "TO_MANY_DEF_IN_MODULE",
            "TO_MANY_USE_IN_MODULE",
            "TO_MANY_INSTR"
    };
    printf("Parse Error line %d offset %d: %s\n", linenum, lineoffset, errstr[errcode].c_str());
}



Tokens readInt() {
    Tokens token = getToken();

    if (token.str == "complete") {
        return token;
    }

    if (token.str == NO_VALUE) {
        parseerror(token.linecount, token.offset, 0);  //num expected
        exit(0);
    }


    for (int i = 0; i < token.str.size(); i++) {
        if((token.str[i]>='0' && token.str[i]<='9')==false) {
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
        parseerror(token.linecount, token.offset, 3); // too many def in module
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
        parseerror(token.linecount, token.offset, 6); // too many def in module
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
        if((token.str[i]>='0' && token.str[i]<='9')==false) {
            parseerror(token.linecount, token.offset, 0); //Addressing expected A/E/I/R
            exit(0);
        }
    }
}

void pass1(string filename) {
    infile.open(filename);
    while (!infile.eof()) {
        /************ define list ***********/
        Tokens deftoken = readInt();
        if (deftoken.str == "complete") {
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
            symboltable[sym] = relativeaddress + base_address;

        }

        /************ use list ***********/

        Tokens usetoken = readInt();
        int usecount = stoi(usetoken.str);
        if (usecount > 16) {
            parseerror(usetoken.linecount, usetoken.offset, 5); // too many def in module
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
        module_num++;
        base_address += instcount;
        complete = true;
    }
    infile.close();
    for (auto p: symboltable ) {
        cout << p.first << ": " << p.second << endl;
    }



}

void pass2(string filename) {
//    infile.open(filename);
//    while (!infile.eof()) {
//        Tokens deftoken = readInt();
//        int defcount = stoi(deftoken.str);
//        for (int i = 0; i < defcount; i++) {
//            string sym = readSym();
//            Tokens valtoken = readInt();
//            int relativeaddress = stoi(valtoken.str);
//
//
//        }
//    }
    return;
}


void parser(string filename) {
    pass1(filename);
    pass2(filename);
}








int main(int argc, char* argv[]) {
    //for(int i=0; i < argc; i++) {// i=1, assuming files arguments are right after the executable
    string fn = argv[1]; //filename
    parser(fn);
    return 0;
}