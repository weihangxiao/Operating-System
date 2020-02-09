#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;


//unordered_map<string, int> symtable;


struct Tokens {
    int linecount;
    int offset;
    string str;
};


queue<Tokens> tokenlist; //store parsed tokenss
//vector<pair<string, int>> symbolTable;


void tokenizer(string filename) {
    ifstream infile;
    infile.open(filename);
    int linenum;
    string line;

   // vector <string> tokens;
    if (infile.is_open()) {
        while (getline(infile, line)) {
            linenum++;
            char *cstr = new char[line.length() + 1];
            strcpy(cstr, line.c_str());
            int loc = -1; //make sure start from the index 0 position
            char *token = strtok(& line[0], " "); //there could be '\t'

            while (token != NULL) {
                loc = line.find(string(token), loc + 1) + 1;
                Tokens tok;
                tok.linecount = linenum;
                tok.offset = loc;
                tok.str = string(token);
                tokenlist.push(tok);
                token = strtok(NULL, " ");

            }
            delete[] cstr;
        }

        //Record the end position


        infile.close();
    }

//    for (int i = 0; i < tokenlist.size(); i++) {
//        cout << tokenlist[i].offset << " " << endl;
//    }

}


int readInt(string str) {
    if (str.size() == 0) return -1;
    for (int i = 0; i < str.size(); i++) {
        if((str[i]>='0' && str[i]<='9')==false) {
            return -1;
        }
    }
    return stoi(str);
}

bool readSym(string str) {
    if (!isalpha(str[0])) {
        return false;
    }

    for (int i = 0; i < str.length(); i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }

    return true;
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

bool parser() {
    int index = 0;
    while (!tokenlist.empty()) {
        Tokens token = tokenlist.front();
        tokenlist.pop();

        if (tokenlist.empty()) {
            parseerror(token.linecount, token.offset, 1); //expected sym
            return false;
        }

        //tokenqueue.pop();

        int defcount = readInt(token.str);
        if (defcount == -1) {
            parseerror(token.linecount, token.offset, 0); // too many def in module
            return false;
        }
        if (defcount > 16) {
            parseerror(token.linecount, token.offset, 4); // too many def in module
            return false;
        }

        for (int i = 0; i < defcount; i++) {
            token = tokenlist.front();
            tokenlist.pop();
            string symbol = token.str;
            if (tokenlist.empty()) {
                parseerror(token.linecount, token.offset, 1); //expected sym     ?????????????
                return false;
            }

            if (!readSym(symbol)) {
                parseerror(token.linecount, token.offset, 1); //expected symbol
                return false;
            }

            if (symbol.length() > 16) {
                parseerror(token.linecount, token.offset, 3); // too many def in module
                return false;
            }


            token = tokenlist.front();
            tokenlist.pop();
            if (tokenlist.empty()) {
                parseerror(token.linecount, token.offset, 0); //expected num
                return false;
            }
            int val = readInt(token.str);
            if (val == -1) {
                parseerror(token.linecount, token.offset, 0); // too many def in module
                return false;
            }

        }

        /***********Use List*************/
        token = tokenlist.front();
        tokenlist.pop();
        if (tokenlist.empty()) {
            parseerror(token.linecount, token.offset, 1); // expected symbol
            return false;
        }

        int usecount = readInt(token.str);
        if (usecount == -1) {
            parseerror(token.linecount, token.offset, 0); // expected num
            return false;
        }

        if (usecount > 16) {
            parseerror(token.linecount, token.offset, 4); // too many def in module
            return false;
        }


        for (int i = 0; i < usecount; i++) {
            token = tokenlist.front();
            tokenlist.pop();
            string symbol = token.str;
            if (symbol.length() > 16) {
                parseerror(token.linecount, token.offset, 3); // too many def in module
                return false;
            }

            if (!readSym(symbol)) {
                parseerror(token.linecount, token.offset, 1); //expected symbol
                return false;
            }
        }


        /*********** Program Text *************/
        token = tokenlist.front();
        tokenlist.pop();



    }

    return true;


}


int main(int argc, char* argv[]) {
    //for(int i=0; i < argc; i++) {// i=1, assuming files arguments are right after the executable
    string fn = argv[1]; //filename
    tokenizer(fn);
    //}

    if (!parser()) {
        return 0;
    }

    return 0;
}