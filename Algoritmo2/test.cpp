#include "Suffix_Array.h"

#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;


string* LecturaArchivo(const string &directorio) {
    ifstream archivo(directorio);
    if (!archivo.is_open()) {
        throw runtime_error("Error al abrir el archivo.");
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    return new string(buffer.str());
}


int main() {

    string* texto;
    try {
        texto = LecturaArchivo("../../dna.100MB.txt");
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1; 
    }

    string patron = "AACC";
    vector<unsigned int>* suffixArray = SuffixArray(texto);





    delete texto;
    return 0;
}
