#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

struct Suffix {
    int indice;
    string sufijo;
};

bool comparacion(const Suffix &s1, const Suffix &s2) {
    if(s1.sufijo < s2.sufijo) {
        return true;
    }
    return false;
}

vector<int> SuffixArray(string texto) {

    int n = texto.length(); 

    Suffix sufijos[n];
    for (int i = 0 ; i < n ; i++) {
        sufijos[i].indice = i;
        sufijos[i].sufijo = texto.substr(i);
    }
    sort(sufijos, sufijos + n, comparacion);

    vector<int> suffixArray;
    for (int i = 0 ; i < n ; i++) {
        suffixArray.push_back(sufijos[i].indice);
    }

    return suffixArray;
}

int contarPatron(string patron, string texto, vector<int> suffixArray) {

    int n = texto.length();
    int m = patron.length();

    int izq = 0;
    int der = n-1;
    int contRep = 0;

    while (izq <= der) {
        int med = izq + (der - izq) / 2;
        string subcadena = texto.substr(suffixArray[med], m);
        if (subcadena == patron) {
            contRep++;
        }
        if (subcadena < patron) {
            izq = med + 1;
        } else {
            der = med - 1;
        }
    }

    return contRep;
}


int main() {
    string texto = "banana";
    string patron = "an";
    vector<int> suffixArray = SuffixArray(texto);

    cout << "SuffixArray para '" << texto << "':" << endl;
    for (int i = 0 ; i < suffixArray.size() ; i++) {
        cout << suffixArray[i] << " ";
    }
    cout << endl;

    int repeticiones = contarPatron(patron, texto, suffixArray);
    cout << "El patron '" << patron << "' se repite " << repeticiones << " veces." << endl;
    
    return 0;
}