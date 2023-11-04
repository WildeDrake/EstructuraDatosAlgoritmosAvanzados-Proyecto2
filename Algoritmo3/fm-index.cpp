#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace sdsl;
using namespace std;
using namespace std::chrono;

class FMIndexSearch {
private:
    string index_suffix = ".fm9";
    size_t max_locations;
    size_t post_context;
    size_t pre_context;

public:
    csa_wt<wt_huff<rrr_vector<127> >, 512, 1024> fm_index;

    FMIndexSearch(size_t max_loc=5, size_t post_ctx=10, size_t pre_ctx=10)
        : max_locations(max_loc), post_context(post_ctx), pre_context(pre_ctx) {}

    // bool constructIndex(const string& file) {
    bool constructIndex() {
        // string index_file = file + index_suffix;
        string index_file = "dna.50MB.txt.fm9";
        string file = "dna.50MB.txt";
        if (load_from_file(fm_index, index_file)) {
            return true;
        }

        ifstream in(file);
        if (!in) {
            cout << "ERROR: File " << file << " does not exist. Exit." << endl;
            return false;
        }

        cout << "No index " << index_file << " located. Building index now." << endl;
        construct(fm_index, file.c_str(), 1); // generate index
        store_to_file(fm_index, index_file);  // save it
        return true;
    }

    void search() {
        cout << "Index construction complete, index requires " << size_in_mega_bytes(fm_index) << " MiB." << endl;
        cout << "Input search terms and press Ctrl-D to exit." << endl;
        string prompt = "\e[0;32m>\e[0m ";
        cout << prompt;
        string query;
        while (getline(cin, query)) {
            displayOccurrences(query);
            cout << prompt;
        }
        cout << endl;
    }

    void displayOccurrences(const string& query) {
        size_t m = query.size();
        size_t occs = sdsl::count(fm_index, query.begin(), query.end());
        cout << "# of occurrences: " << occs << endl;
        if (occs > 0) {
            cout << "Location and context of first occurrences: " << endl;
            auto locations = locate(fm_index, query.begin(), query.begin() + m);
            sort(locations.begin(), locations.end());
            for (size_t i = 0, pre_extract = pre_context, post_extract = post_context; i < min(occs, max_locations); ++i) {
                cout << setw(8) << locations[i] << ": ";
                if (pre_extract > locations[i]) {
                    pre_extract = locations[i];
                }
                if (locations[i] + m + post_extract > fm_index.size()) {
                    post_extract = fm_index.size() - locations[i] - m;
                }
                auto s = extract(fm_index, locations[i] - pre_extract, locations[i] + m + post_extract - 1);
                string pre = s.substr(0, pre_extract);
                s = s.substr(pre_extract);
                if (pre.find_last_of('\n') != string::npos) {
                    pre = pre.substr(pre.find_last_of('\n') + 1);
                }
                cout << pre;
                cout << "\e[1;31m";
                cout << s.substr(0, m);
                cout << "\e[0m";
                string context = s.substr(m);
                cout << context.substr(0, context.find_first_of('\n')) << endl;
            }
        }
    }
};


struct tiempos {
    unsigned int PreComputo, ContPatron, SumTotal;
};

tiempos Test(string* &texto, string patron) {
    size_t max_locations = 5;
    size_t post_context = 10;
    size_t pre_context = 10;

    tiempos resultados;
    auto start = high_resolution_clock::now();
    FMIndexSearch fmSearch(max_locations, post_context, pre_context);
    if (!fmSearch.constructIndex()) {
        cout << "Ha ocurrido un error en la inicialización del FM-index\n";
    }
    auto end = high_resolution_clock::now();

    resultados.PreComputo =  duration_cast<microseconds>(end - start).count();

    start = high_resolution_clock::now();
    size_t occs = sdsl::count(fmSearch.fm_index, patron.begin(), patron.end());
    end = high_resolution_clock::now();
    resultados.ContPatron =  duration_cast<microseconds>(end - start).count();

    resultados.SumTotal = resultados.ContPatron + resultados.PreComputo;

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

