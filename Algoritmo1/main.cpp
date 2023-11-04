#include <iostream>
#include "BMH.h"
#include <string>
using namespace std;

int main(){
    BMH a1, a2, a3, a4, a5, a6, a7;
    string *text = new string("TRUSTHARDTOOTHBRUSHES");
    string *text2 = new string("TOOTH TOOTH");
    string *text3 = new string("(Black screen with text); The sound of buzzing bees can be heard) According to all known laws of aviation, there is no way a bee should be able to fly. Its wings are too small to get its fat little body off the ground. The bee, of course, flies anyway (...)");
    string *text4 = new string("ABABABABABBABAB");
    string *text5 = new string("AABAACAADAABAABA");
    string *text6 = new string("be be be be be be be be be be");
    a1.createPT("TOOTH");
    a2.createPT("TOOTH");
    a3.createPT("bee");
    a4.createPT("AB");
    a5.createPT("AABA");
    a6.createPT("A");
    a7.createPT("be");
    cout<<a1.count(text,"TOOTH")<<endl;
    cout<<a2.count(text2,"TOOTH")<<endl;
    cout<<a3.count(text3,"bee")<<endl;
    cout<<a4.count(text4,"AB")<<endl;
    cout<<a5.count(text5,"AABA")<<endl;
    cout<<a6.count(text5,"A")<<endl;
    cout<<a7.count(text6,"be")<<endl;
}
