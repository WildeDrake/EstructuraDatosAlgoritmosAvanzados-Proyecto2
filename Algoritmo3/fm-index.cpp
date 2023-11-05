#include <sdsl/suffix_arrays.hpp>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <chrono>

using namespace sdsl;
using namespace std;
using namespace std::chrono;

/* string original_file; */

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
    // bool constructIndex(int maxsize) {
    string* constructIndex() {
        // string index_file = file + index_suffix;
        string index_file = "dna.50MB.txt.fm9";
        string file = "dna.50MB.txt";
        // if (load_from_file(fm_index, index_file)) {
        //     // cout << fm_index.size() << endl;
        //     return true;
        // }

        ifstream in(file);
        if (!in) {
            cout << "ERROR: File " << file << " does not exist. Exit." << endl;
            return nullptr;
        }

        cout << "No index " << index_file << " located. Building index now." << endl;
        // char *s = (char*)malloc((maxsize+1) * sizeof(char*));
        // copy(file.begin(), file.begin()+maxsize, s);
        construct(fm_index, file.c_str(), 1); // generate index
        // construct(fm_index, s, 1); // generate index
        store_to_file(fm_index, index_file);  // save it
        string ret(file.c_str());
        return &ret;
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

tiempos Test(FMIndexSearch &fmSearch, string patron) {

    tiempos resultados;

    // resultados.PreComputo =  duration_cast<microseconds>(end - start).count();
    resultados.PreComputo =  0;

    auto start = high_resolution_clock::now();
    size_t occs = sdsl::count(fmSearch.fm_index, patron.begin(), patron.end());
    auto end = high_resolution_clock::now();
    resultados.ContPatron =  duration_cast<microseconds>(end - start).count();

    resultados.SumTotal = resultados.ContPatron + resultados.PreComputo;

    return resultados;
} 

string parseChar(int n) {
    switch (n) {
        case 0:
            return "A";
        case 1:
            return "C";
        case 2:
            return "T";
        case 3:
            return "G";
    }
    cout << "Hay algo raro pasando al crear el string\n";
    return "\0";
}

string getRandomString(int size) {
    string s = "";
    mt19937 mt;
    for (int i=0;i<size;++i) {
        s += parseChar(mt()%4);
    }
    return s;
}


int main(int argc, char* argv[]) {

    if(argc != 2) {
        cout << "Uso: " << argv[0] << " n" << endl;
        return 1;
    }
    int tamStr = atoi(argv[1]);
    
    // string* texto;
    // try {
    //     texto = LecturaArchivo("../dna.50MB.txt", tamStr); // Alternativamente "../../dna.100MB.txt"
    // } catch (const exception& e) {
    //     cerr << e.what() << endl;
    //     return 1; 
    // }

    size_t max_locations = 5;
    size_t post_context = 10;
    size_t pre_context = 10;
    FMIndexSearch fmSearch(max_locations, post_context, pre_context);
    string original = *fmSearch.constructIndex();
    // if (!fmSearch.constructIndex()) {
    //     cout << "Ha ocurrido un error en la inicialización del FM-index\n";
    // }

    double promPre = 0, promCont = 0, promTot = 0, varPre = 0, varCont = 0, varTot = 0;
    vector<tiempos> resultados;
    // string tested_string = getRandomString(tamStr);
    string tested_string = original.substr(0, tamStr);
    // cout << "tested_string size: " << tested_string.size() << endl;
    //cout << tested_string << endl;
    const int tests = 300;
    for(int i = 0 ; i < tests ; ++i){
        // resultados.push_back(Test(texto, "AACCTA"));
        resultados.push_back(Test(fmSearch, tested_string));
        promPre += resultados.at(i).PreComputo;
        promCont += resultados.at(i).ContPatron;
        promTot += resultados.at(i).SumTotal;
    }
    promPre = promPre / tests;
    promCont = promCont / tests;
    promTot = promTot / tests;

    for(int i = 0 ; i < tests ; ++i){
        varPre += (static_cast<double>(resultados.at(i).PreComputo) - promPre) * (static_cast<double>(resultados.at(i).PreComputo) - promPre);
        varCont += (static_cast<double>(resultados.at(i).ContPatron) - promCont) * (static_cast<double>(resultados.at(i).ContPatron) - promCont);
        varTot += (static_cast<double>(resultados.at(i).SumTotal) - promTot) * (static_cast<double>(resultados.at(i).SumTotal) - promTot);
    }
    varPre = varPre / tests;
    varCont = varCont / tests;
    varTot = varTot / tests;
    
    cout << tamStr << ";" << fixed << setprecision(0) << promPre << ";" << fixed << setprecision(0) << promCont << ";" << fixed << setprecision(0) 
    << promTot << ";" << fixed << setprecision(0) << varPre << ";" << fixed << setprecision(0) << varCont << ";" << fixed << setprecision(0) << varTot << endl;

    return 0;
}

