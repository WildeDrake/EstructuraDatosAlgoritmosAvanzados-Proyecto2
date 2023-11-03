#include "BMH.h"

#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>

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


struct tiempos {
    unsigned int PreComputo, ContPatron, SumTotal, size;
};

tiempos Test(string* &texto, string patron) {
    BMH bmh;
    tiempos resultados;

    auto start = high_resolution_clock::now();
    bmh.createPT(patron);
    auto end = high_resolution_clock::now();
    resultados.PreComputo =  duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    bmh.count(texto, patron);
    end = high_resolution_clock::now();
    resultados.ContPatron =  duration_cast<microseconds>(end - start).count();

    resultados.SumTotal = resultados.ContPatron + resultados.PreComputo;
    resultados.size = sizeof(bmh.PTBad);

    delete[] bmh.PTBad;
    return resultados;
} 


int main() {

    string* texto;
    try {
        texto = LecturaArchivo("../dna.50MB.txt"); // Alternativamente "../../dna.100MB.txt"
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1; 
    }

    vector<tiempos> resultados;
    for(int i = 0 ; i < 30 ; ++i){
        resultados.push_back(Test(texto, "AACCT"));
    }

    unsigned int PreCompTotal = 0, PreCompCuadrados = 0, ContPatrTotal = 0, ContPatrCuadrados = 0, sumaTotal = 0, TotalCuadrados = 0;
    for (const auto& tiempo : resultados) {
        PreCompTotal += tiempo.PreComputo;
        PreCompCuadrados += tiempo.PreComputo * tiempo.PreComputo;
        ContPatrTotal += tiempo.ContPatron;
        ContPatrCuadrados += tiempo.ContPatron * tiempo.ContPatron;
        sumaTotal += tiempo.SumTotal;
        TotalCuadrados += tiempo.SumTotal * tiempo.SumTotal;
    }
    double promPre = static_cast<double>(PreCompTotal) / resultados.size();
    double promCont = static_cast<double>(ContPatrTotal) / resultados.size();
    double promTot = static_cast<double>(sumaTotal) / resultados.size();
    double varPre = static_cast<double>(PreCompCuadrados) / resultados.size() - promPre * promPre;
    double varCont = static_cast<double>(ContPatrCuadrados) / resultados.size() - promCont * promCont;
    double varTot = static_cast<double>(TotalCuadrados) / resultados.size() - promTot * promTot;

    cout << "Tiempo PreComputo;Tiempo Contar Patron Repetido;Tiempo Total;Tamaño" << "\n";
    for(int i = 0 ; i < 30 ; ++i){
        cout << resultados.at(i).PreComputo << ";" << resultados.at(i).ContPatron << ";" << resultados.at(i).SumTotal << ";" << resultados.at(i).size << "\n";
    }
    cout << "Promedio;Promedio;Promedio" << "\n";
    cout << promPre << ";" << promCont << ";" << promTot << "\n";
    cout << "Varianza;Varianza;Varianza" << "\n";
    cout << varPre << ";" << varCont << ";" << varTot << "\n";

    delete texto;
    return 0;
}