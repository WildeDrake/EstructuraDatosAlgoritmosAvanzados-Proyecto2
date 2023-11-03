#include "Suffix_Array.h"

int main() {
    string* texto = new string("banana");
    string patron = "a";
    vector<unsigned int>* suffixArray = SuffixArray(texto);

    cout << "SuffixArray para '" << *texto << "':" << endl;
    for (int i = 0 ; i < suffixArray->size() ; i++) {
        cout << suffixArray->at(i) << " ";
    }
    cout << endl;

    int repeticiones = contarPatron(patron, texto, suffixArray);
    cout << "El patron '" << patron << "' se repite " << repeticiones << " veces." << endl;
    
    return 0;
}