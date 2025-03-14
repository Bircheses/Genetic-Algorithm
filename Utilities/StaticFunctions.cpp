#include <fstream>
#include <iomanip>
#include <iostream>
#include <ctime>
#include <cmath>
#include <thread>

#include "GeneticAlgorithm.h"

using namespace std;

static int** generate_matrix_asymetric(int size){
    srand(time(NULL));
    //inicjalizacja macierzy
    int** matrix = new int*[size];
    for(int i=0; i<size; i++){
        matrix[i] = new int[size];
    }
    //uzupełnienie macierzy losowymi wartościami krawędzi
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++) {
            if(i==j) matrix[i][j] = -1;
            else matrix[i][j] = rand()%100+1;
        }
    }
    return matrix;
}

static std::pair<int, int*> generate_matrix_symetric(int size){
    srand(time(NULL));

    //inicjalizacja tablicy
    int size_temp = size*(size-1)/2;
    int* tab = new int[size_temp];

    for(int i=0; i<size_temp; i++) {
        tab[i] = rand()%100+1;
    }

    return std::make_pair(size_temp, tab);
}

static void show_matrix(int** matrix, int size){
    cout << "Macierz: " << endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            cout << setw(3) << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

static void show_tab(int* tab, int tabsize){
    cout << "Tablica: " << endl;
    for (int i=0; i<tabsize; i++) {
        cout << tab[i] << " ";
    }
    cout << endl;
}

static void show_tour (int* tour, int size) {
    for(int i=0; i<size; i++) {
        cout << tour[i] << "->";
    }
    cout << tour[0] << endl;
}

static void delete_matrix(int** matrix, int size){
    if(matrix == nullptr) return;
    for(int i=0; i<size; i++){
        delete [] matrix[i];
    }
    delete [] matrix;
}

// Function to read ATSP (Asymmetric TSP) file and create a distance matrix
static pair<int, int**> read_atsp(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return std::make_pair(-1, nullptr);
    }

    string line, key;
    int** matrix = nullptr;
    bool is_edge_weight_section = false;
    int size = 0;

    int i=0, j=0;

    while (getline(file, line)) {
        stringstream ss(line);

        ss >> key;

        if (key == "DIMENSION:") {
            ss >> size;
            matrix = new int*[size];
            for (int i = 0; i < size; i++) {
                matrix[i] = new int[size];
            }
        } else if (key == "EDGE_WEIGHT_SECTION") {
            is_edge_weight_section = true;
            continue;
        } else if (key == "EOF") {
            break;
        }

        if (is_edge_weight_section) {
            istringstream stream(line);
            int value;
            while(stream >> value) {
                if(i==size) {
                    i=0;
                    j++;
                }
                matrix[j][i] = value;
                i++;
            }
        }
    }

    file.close();
    return std::make_pair(size, matrix);
}

static int silnia(int size){
    if(size == 2) return 2;
    return size*silnia(size-1);
}

/**
 * Reading data (matrix) from file
 * @param path - path to destination file
 * @return address of matrix
 */
static pair<int, int**> read_from_file (string path) {
    int** matrix = nullptr;
    int temp, i=0, j=0, k=0, amount=0;
    ifstream file(path);
    if(!file.is_open()) return std::make_pair(-1,nullptr);;
    while (file >> temp) {
        if(i == 0) {
            amount = temp;
            matrix = new int*[amount];
            for(int i=0; i<amount; i++){
                matrix[i] = new int[amount];
            }
        }else if(k<amount){
            matrix[j][k] = temp;
            k++;
        }
        if(k==amount) {
            k=0;
            j++;
        }
        i=1;
    }
    return std::make_pair(amount, matrix);
}

static void write_csv_file(double wsp_crossing, double wsp_mutation, int cross_strat, int mutate_strat,  return_values rv, int size) {
    ofstream file("results.csv", std::ios::app);
    string cross_strat_string = "";
    string mutate_strat_string = "";

    if(cross_strat == 0) cross_strat_string = "PMX";
    else if(cross_strat == 1) cross_strat_string = "0X";

    if(mutate_strat == 0) mutate_strat_string = "swap";
    else if(mutate_strat == 1) mutate_strat_string = "insert";

    file << rv.cost << ";" << rv.time << ";" << cross_strat_string << ";" << wsp_crossing << ";" << mutate_strat_string << ";" << wsp_mutation << ";";

    for(int i=0; i<size; i++) {
        file << rv.tour[i] << " ";
    }

    file << endl;
}

static void start_thread(int** matrix, int size, int cross_strat, int mutate_strat, int population, double stop_time, double wsp_crossing, double wsp_mutation) {
    return_values rv;

    GeneticAlgorithm GA;

    GA.load_matrix(matrix, size);

    rv = GA.genetic_algorithm(stop_time, mutate_strat, cross_strat, population, wsp_mutation, wsp_crossing);

    write_csv_file(wsp_crossing, wsp_mutation, cross_strat, mutate_strat, rv, size);

    delete[] rv.tour;
}

static void read_conf_file () {
    int size;
    int** matrix;
    double stop_time;
    int crossing;
    double wsp_crossing;
    int mutation;
    double wsp_mutation;
    int population_size;
    int tournament_size;
    double new_gen_intake;

    std::ifstream file("config.txt");
    if (!file.is_open()) {
        std::cerr << "Nie udalo sie otworzyc pliku konfiguracyjnego" << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line.find('#') == 0) {
            continue;
        }

        if (!line.find("nazwa_pliku:")) {
            istringstream iss(line);
            string command = "";
            string argument = "";
            iss >> command >> argument;

            auto [fst, scd] = read_atsp(argument);
            size = fst;
            matrix = scd;
        }

        if (!line.find("czas_stopu:")) {
            istringstream iss(line);
            string command = "";
            double argument;
            iss >> command >> argument;

            stop_time = argument * 1000; // Converting to milliseconds
        }

        if (!line.find("crossing:")) {
            istringstream iss(line);
            string command = "";
            string argument;
            iss >> command >> argument >> wsp_crossing;

            string arg1 = argument.substr(0, argument.find(';'));

            if(arg1 == "PMX") {
                crossing = 0;
            }else crossing = 1;
        }

        if (!line.find("mutation:")) {
            istringstream iss(line);
            string command = "";
            string argument;
            iss >> command >> argument >> wsp_mutation;

            string arg1 = argument.substr(0, argument.find(';'));

            if(arg1 == "swap") {
                mutation = 0;
            }else mutation = 1;
        }

        if (!line.find("population:")) {
            istringstream iss(line);
            string command = "";
            int argument;
            iss >> command >> argument;

            population_size = argument;
        }

        if (!line.find("tournament_size:")) {
            istringstream iss(line);
            string command = "";
            int argument;
            iss >> command >> argument;

            tournament_size = argument;
        }

        if (!line.find("old_best_gen_intake:")) {
            istringstream iss(line);
            string command = "";
            double argument;
            iss >> command >> argument;

            new_gen_intake = 1 - argument;
        }

        if (!line.find("SA:")) {
            istringstream iss(line);
            string command = "";
            int argument;
            iss >> command >> argument;

            if(argument == 0) {

            }else if(argument == 1) {

            }else if(argument == 2) {

            }
        }

        if (!line.find("Simulate")) {
            ofstream file("results.csv", std::ios::app);

            int algorithm = 0;
            int mutations[2] = {0, 1};
            int crossings[2] = {0, 1};
            double stop_times[3] = {60000, 120000, 240000};
            double wsp_cross[3] = {0.5, 0.7, 0.9};
            double wsp_mutate[3] = {0.01, 0.05, 0.1};
            double population[3] = {16, 57, 134};

            int** matrix;
            int size;

            if(algorithm == 0) {
                auto [f1, s1] = read_atsp("ftv47.atsp");
                matrix = s1;
                size = f1;
            }else if(algorithm == 1) {
                auto [f1, s1] = read_atsp("ftv170.atsp");
                matrix = s1;
                size = f1;
            }else if(algorithm == 2) {
                auto [f1, s1] = read_atsp("rbg403.atsp");
                matrix = s1;
                size = f1;
            }

            //Wybór krzyżowania
            for (int & crossing1 : crossings) {
                //Wybór mutacji
                for (int & mutation1 : mutations) {
                    //Wybór wsp krzyżowania
                    for(double & wsp_cros : wsp_cross) {
                        //Wybór wsp mutacji
                        for(double & m : wsp_mutate) {
                            //2 iteracje po 5 threadów = 10 wyników
                            for (int l = 0; l < 2; l++) {
                                thread t1(start_thread, matrix, size, crossing1, mutation1, population[algorithm],
                                          stop_times[algorithm], wsp_cros, m);

                                this_thread::sleep_for(std::chrono::milliseconds(200));

                                thread t2(start_thread, matrix, size, crossing1, mutation1, population[algorithm],
                                          stop_times[algorithm], wsp_cros, m);

                                this_thread::sleep_for(std::chrono::milliseconds(200));

                                thread t3(start_thread, matrix, size, crossing1, mutation1, population[algorithm],
                                          stop_times[algorithm], wsp_cros, m);

                                this_thread::sleep_for(std::chrono::milliseconds(200));

                                thread t4(start_thread, matrix, size, crossing1, mutation1, population[algorithm],
                                          stop_times[algorithm], wsp_cros, m);

                                this_thread::sleep_for(std::chrono::milliseconds(200));

                                thread t5(start_thread, matrix, size, crossing1, mutation1, population[algorithm],
                                          stop_times[algorithm], wsp_cros, m);

                                t1.join();
                                t2.join();
                                t3.join();
                                t4.join();
                                t5.join();
                            }
                        }
                    }
                }
            }
        }

    }

    /*GeneticAlgorithm GA;
    GA.load_matrix(matrix, size);
    int cost = GA.genetic_algorithm(stop_time, mutation, crossing, population_size, wsp_mutation, wsp_crossing, tournament_size, new_gen_intake);

    cout << cost << endl;*/

    delete_matrix(matrix, size);
    file.close();
}