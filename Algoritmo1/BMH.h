#ifndef BMH_H
#define BMH_H

#include <iostream>
#include <string>

using namespace std;

class BMH{
    public:
        int* PTBad;
        BMH();
        void createPT(string pattern);      
        int count(string *TEXT, string pattern);
        bool search(string* text, int t, string pattern, int count);
        ~BMH();
};

#endif