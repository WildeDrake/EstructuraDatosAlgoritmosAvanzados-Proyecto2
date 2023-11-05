#include <iostream>
#include <string>

using namespace std;

bool search(string *text, int t, string pattern, int count) { //Algoritmo que compara si los chars son iguales, de atrás para adelante
    if (text->at(t) == pattern[pattern.length() - count - 1]) {
        return true; //Si son iguales, retorna true
    } else {
        return false; //Si no, false
    }
}

void createPT(string pattern, int* &PTBad) {
    PTBad = new int[256];
    int default_value = pattern.length();

    // Llenar tabla de chars size = pattern.length()
    for(int i = 0; i < 256; ++i) {
        PTBad[i] = pattern.length();
    }

    // Llenar tabla de patron, excepto el ultimo
    for(int i = 0; i < pattern.length() - 1; ++i) {
        PTBad[(int)pattern[i]] = pattern.length() - i - 1;
    }
}

int count(string *text, string pattern, int* PTBad) {
    int textp = pattern.length() - 1;
    int count = 0;
    int count_coincidencia = 0;
    while((int)textp < (int)text->length()) {
        //cout<<textp<<","<<text->at(textp)<<","<<pattern[pattern.length()-count_coincidencia-1]<<","<<count_coincidencia<<endl;
        if ((int)count_coincidencia == (int)pattern.length()) {
            ++count;
            textp = textp + pattern.length() + 1;
            count_coincidencia = 0;
        } else {
            if (search(text, textp, pattern, count_coincidencia)) {
                textp = textp - 1;
                ++count_coincidencia;
                //cout<<textp<<","<<count_coincidencia<<endl;
            } else {
                textp = textp + PTBad[(int)text->at(textp)] + count_coincidencia;
                count_coincidencia = 0;
                //cout<<textp<<endl;
            }
        }
    }
    return count;
}


