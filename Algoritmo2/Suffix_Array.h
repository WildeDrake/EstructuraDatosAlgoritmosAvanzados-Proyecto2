// Autor Diego Venegas :] 
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string_view>

using namespace std;


vector<unsigned int>* SuffixArray(string* &texto) {
    int n = texto->length();
    string_view substring = string_view(*texto);
    vector<unsigned int> *suffixArray = new vector<unsigned int>();

    for(int i = 0 ; i < n ; i++) {
        suffixArray->push_back(i);
    }

    sort(suffixArray->begin(), suffixArray->end(), [substring](unsigned l, unsigned r) -> bool {
        return substring.substr(l) < substring.substr(r);
    });

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