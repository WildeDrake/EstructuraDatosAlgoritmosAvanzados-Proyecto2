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

int contarPatron(string &patron, string &texto, vector<int> &suffixArray) {
    int izq = 0;
    int der = texto.length() - 1;
    int cont = 0;

    while (izq <= der) {
        int mid = izq + (der - izq) / 2;
        string sufijo = texto.substr(suffixArray[mid], patron.length());

        if (sufijo == patron) { // Coincidencia lexicografica.
            cont++;
            // Buscar ocurrencias al lado izquierdo.
            int i = mid - 1;
            while (i >= izq && texto.substr(suffixArray[i], patron.length()) == patron) {
                cont++;
                i--;
            }
            // Buscar ocurrencias al lado derecho.
            i = mid + 1;
            while (i <= der && texto.substr(suffixArray[i], patron.length()) == patron) {
                cont++;
                i++;
            }
            break;

        } else if (sufijo < patron) { // Patron menor lexicograficamente.
            izq = mid + 1;
            
        } else { // Patron mayor lexicograficamente.
            der = mid - 1;
        }
    }
    
    return cont;
}

int main() {
    string texto = "banana";
    string patron = "ana";
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