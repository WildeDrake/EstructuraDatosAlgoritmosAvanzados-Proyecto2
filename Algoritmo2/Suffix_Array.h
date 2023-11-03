#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;


struct Suffix {
    int indice;
    string sufijo;
};


bool comparacion(const Suffix* s1, const Suffix* s2) {
    return s1->sufijo < s2->sufijo;
}


vector<unsigned int>* SuffixArray(string* &texto) {
    int n = texto->length(); 

    vector<Suffix*>* sufijos = new vector<Suffix*>();
    for(int i = 0 ; i < n ; i++) {
        Suffix* sufijo = new Suffix();
        sufijo->indice = i;
        sufijo->sufijo = texto->substr(i);
        sufijos->push_back(sufijo);
    }
    sort(sufijos->begin(), sufijos->end(), &comparacion);

    vector<unsigned int> *suffixArray = new vector<unsigned int>();
    for (int i = 0 ; i < n ; i++) {
        suffixArray->push_back((*sufijos)[i]->indice);
    }

    while (sufijos->empty() == 0) {
        delete sufijos->back();
        sufijos->pop_back();
    }
    delete sufijos;

    return suffixArray;
}


int contarPatron(string &patron, string* &texto, vector<unsigned int>* suffixArray) {
    int izq = 0;
    int der = texto->length() - 1;
    int cont = 0;

    while (izq <= der) {
        int med = izq + (der - izq) / 2;
        string sufijo = texto->substr(suffixArray->at(med), patron.length());

        if (sufijo == patron) { // Coincidencia lexicografica.
            cont++;
            // Buscar ocurrencias al lado izquierdo.
            int i = med - 1;
            while (i >= izq && texto->substr(suffixArray->at(i), patron.length()) == patron) {
                cont++;
                i--;
            }
            // Buscar ocurrencias al lado derecho.
            i = med + 1;
            while (i <= der && texto->substr(suffixArray->at(i), patron.length()) == patron) {
                cont++;
                i++;
            }
            break;

        } else if (sufijo < patron) { // Patron menor lexicograficamente.
            izq = med + 1;
            
        } else { // Patron mayor lexicograficamente.
            der = med - 1;
        }
    }
    
    return cont;
}