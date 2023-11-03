#include "BMH.h"

using namespace std;


BMH::BMH(){
}


void BMH::createPT(string &pattern){
    PTBad = new int[256];
    int default_value = pattern.length();
    // Llenar tabla de chars size = pattern.length()
    for(int i = 0; i < 256; ++i){
        PTBad[i] = pattern.length();
    }
    // Llenar tabla de patron, excepto el ultimo
    // El ultimo tiene de valor pattern.length() si no esta definido
    // Y si esta definido, ocupa ese valor

    for(int i = 0 ; i < pattern.length() - 1 ; ++i ){
        PTBad[(int)pattern[i]] = pattern.length() - i - 1;
    }
}


int BMH::count(string *text, string &pattern){
    //createPT(pattern);                // Crea la Pattern Table
    int textp = pattern.length() - 1;   // Indice de donde se esta analizando en el txt, en BHM siempre se empieza primero
                                        // con la ultima letra del patron, por lo que se empieza desde length() - 1
    int count = 0;
    int count_coincidencia = 0;
    while((int)textp < (int)text->length()){ // Analiza todo el texto, hasta que el indice llegue al final      
        if(count_coincidencia == pattern.length()){  // Confirma si hay suficientes coincidencias para declararlo como un patron completo
            ++count;
            textp = textp + pattern.length() + 1;
            count_coincidencia = 0;
        } else {
            if(search(text, textp, pattern, count_coincidencia)){
                textp = textp - 1;
                ++count_coincidencia;
            } else {
                textp = textp + PTBad[(int) text->at(textp)];
                count_coincidencia = 0;
            }
        }
    }
    return count;
}


bool BMH::search(string *text, int t, string &pattern, int count) {
    if (text->at(t) == pattern[pattern.length() - count - 1]) {
        return true;
    } else {
        return false;
    }

}


BMH::~BMH(){
    delete[] PTBad;
}