#include "BMH.h"

#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std::chrono;


string* LecturaArchivo(const string &directorio, int tamStr) {
    ifstream archivo(directorio);
    if (!archivo.is_open()) {
        throw runtime_error("Error al abrir el archivo.");
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    return new string(buffer.str().substr(0, tamStr));
}


struct tiempos {
    unsigned int PreComputo, ContPatron, SumTotal;
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

    delete[] bmh.PTBad;
    return resultados;
} 


int main(int argc, char* argv[]) {

    if(argc != 2) {
        cout << "Uso: " << argv[0] << " n" << endl;
        return 1;
    }
    int tamStr = atoi(argv[1]);
    
    string* texto;
    try {
        texto = LecturaArchivo("../dna.50MB.txt", tamStr); // Alternativamente "../../dna.100MB.txt"
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1; 
    }

    double promPre = 0, promCont = 0, promTot = 0, varPre = 0, varCont = 0, varTot = 0;
    vector<tiempos> resultados;
    for(int i = 0 ; i < 30 ; ++i){
        resultados.push_back(Test(texto, "AACCTA"));
        promPre += resultados.at(i).PreComputo;
        promCont += resultados.at(i).ContPatron;
        promTot += resultados.at(i).SumTotal;
    }
    promPre = promPre / 30;
    promCont = promCont / 30;
    promTot = promTot / 30;

    for(int i = 0 ; i < 30 ; ++i){
        varPre += (static_cast<double>(resultados.at(i).PreComputo) - promPre) * (static_cast<double>(resultados.at(i).PreComputo) - promPre);
        varCont += (static_cast<double>(resultados.at(i).ContPatron) - promCont) * (static_cast<double>(resultados.at(i).ContPatron) - promCont);
        varTot += (static_cast<double>(resultados.at(i).SumTotal) - promTot) * (static_cast<double>(resultados.at(i).SumTotal) - promTot);
    }
    varPre = varPre / 30;
    varCont = varCont / 30;
    varTot = varTot / 30;
    
    cout << tamStr << ";" << fixed << setprecision(0) << promPre << ";" << fixed << setprecision(0) << promCont << ";" << fixed << setprecision(0) 
    << promTot << ";" << fixed << setprecision(0) << varPre << ";" << fixed << setprecision(0) << varCont << ";" << fixed << setprecision(0) << varTot << endl;

    delete texto;
    return 0;
}