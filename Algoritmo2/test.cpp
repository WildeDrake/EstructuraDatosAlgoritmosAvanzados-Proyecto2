#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    ifstream archivo("../dna.100MB.txt");
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo." << endl;
        return 1;
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    string texto = buffer.str();

    cout << texto << endl;

    return 0;
}
