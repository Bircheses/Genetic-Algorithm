#include "../Headers/GeneticAlgorithm.h"
#include "../Headers/Counter.h"
#include "../Utilities/StaticFunctions.cpp"
#include <ctime>
#include <cmath>


bool GeneticAlgorithm::find(int *tour, int begin, int end, int value) {
    for(int i=begin; i<end; i++) {
        if(tour[i] == value) {
            return true;
        }
    }
    return false;
}

double_tour GeneticAlgorithm::OX(int *tour1, int *tour2, int size, int begin, int end) {
    int* new_tour1 = new int[size];
    int* new_tour2 = new int[size];

    for(int k=begin; k<end; k++) {
        new_tour1[k] = tour2[k];
        new_tour2[k] = tour1[k];
    }

    int index1 = end % size;
    int index2 = end % size;

    for (int k = 0; k < size; k++) {
        int current_val1 = tour1[(end + k) % size];
        int current_val2 = tour2[(end + k) % size];

        if (!find(new_tour1, begin, end, current_val2)) {
            new_tour1[index1] = current_val2;
            index1 = (index1 + 1) % size;
        }

        if (!find(new_tour2, begin, end, current_val1)) {
            new_tour2[index2] = current_val1;
            index2 = (index2 + 1) % size;
        }
    }

    double_tour dt{new_tour1, new_tour2};
    return dt;
}

double_tour GeneticAlgorithm::PMX(int *tour1, int *tour2, int size, int i, int j) {
    int* new_tour1 = new int[size];
    int* new_tour2 = new int[size];

    for(int i=0; i<size; i++) {
        new_tour1[i] = -1;
        new_tour2[i] = -1;
    }

    for(int k=i; k<j; k++) {
        new_tour1[k] = tour2[k];
        new_tour2[k] = tour1[k];
    }

    for(int k=0; k<size; k++) {
        if(k>=i && k<j) continue;

        if(!find(tour1, i, j, tour2[k])) {
            new_tour2[k] = tour2[k];
        }

        if(!find(tour2, i, j, tour1[k])) {
            new_tour1[k] = tour1[k];
        }
    }

    for(int k=0; k<size; k++) {
        if(k>=i && k<j) continue;

        if(!find(new_tour1, 0, size, tour2[k])) {
            int l=0;
            while(new_tour1[l]!=-1){
                l++;
            }
            new_tour1[l]=tour2[k];
        }

        if(!find(new_tour2, 0, size, tour1[k])) {
            cout << tour1[k] << endl;
            int l=0;
            while(new_tour2[l]!=-1){
                l++;
            }
            new_tour2[l] = tour1[k];
        }
    }

    double_tour dt{new_tour1, new_tour2};
    return dt;
}

void GeneticAlgorithm::swap(int *tour, int i, int j) {
    std::swap(tour[i], tour[j]);
}

void GeneticAlgorithm::insert(int *tour, int i, int j) {
    int city = tour[i];
    if(i < j) {
        for(int k = i; k < j; ++k) {
            tour[k] = tour[k+1];
        }
    } else {
        for(int k = i; k > j; --k) {
            tour[k] = tour[k-1];
        }
    }
    tour[j] = city;
}

void GeneticAlgorithm::inverse(int *tour, int i, int j) {
    if(i < j){
        while(i < j) {
            std::swap(tour[i], tour[j]);
            ++i;
            --j;
        }
    }else{
        while(i > j) {
            std::swap(tour[i], tour[j]);
            --i;
            ++j;
        }
    }
}

int * GeneticAlgorithm::copy(const int *tour, int size) {
    int* temp = new int[size];
    for(int i=0; i<size; i++) temp[i] = tour[i];
    return temp;
}

int GeneticAlgorithm::calculate_cost(int **matrix, int *tour, int size) {
    int totalCost = 0;
    for (int i = 0; i < size-1; ++i) {
        totalCost += matrix[tour[i]][tour[i+1]];
    }
    totalCost += matrix[tour[size-1]][tour[0]];
    return totalCost;
}

int * GeneticAlgorithm::generate_random_tour(int size) {
    int* a = new int[size];
    for(int i=0; i<size; i++) {
        a[i] = i;
    }
    for(int i=0; i<size; i++) {
        int random = rand() % size;
        std::swap(a[i], a[random]);
    }
    return a;
}

int GeneticAlgorithm::genetic_algorithm(double stop_time, int mutation_strategy, int crossing_strategy, int population_size, double wsp_mut, double wsp_cros) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);// CLOCK_MONOTONIC gwarantuje stały wzrost czasu
    srand(ts.tv_nsec ^ ts.tv_sec);

    Counter counter;
    counter.start();

    int bestCost = INT32_MAX;

    double time_found = 0;
    int ** tour_array = new int*[population_size];
    for(int i=0; i<population_size; i++){
        tour_array[i] = generate_random_tour(size);
    }

    while(counter.getElapsedTime() < stop_time){
        int i = rand() % size;
        int j = rand() % size;
        while (i == j) j = rand() % size;

        if(wsp_cros > (rand() / (double)RAND_MAX)){

        }

        if(wsp_mut > (rand() / (double)RAND_MAX)){

        }
    }

    for(int i=0; i<population_size; i++){
        delete [] tour_array[i];
    }
    delete [] tour_array;

    return 0;
}

void GeneticAlgorithm::load_matrix(int **matrix, int size) {
    if(this->matrix!=nullptr) {
        for(int i=0; i<this->size; i++) delete [] this->matrix[i];
        delete [] this->matrix;
    }
    this->size = size;

    //initialize matrix
    this->matrix = new int*[size];
    for(int i=0; i<size; i++) {
        this->matrix[i] = new int[size];
    }

    //copy values from old address to new address
    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            this->matrix[i][j] = matrix[i][j];
        }
    }
}

GeneticAlgorithm::~GeneticAlgorithm() {
    if(this->matrix == nullptr) return;
    for(int i=0; i<size; i++) delete [] this->matrix[i];
    delete [] this->matrix;
}
