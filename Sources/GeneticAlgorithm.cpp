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

double_tour GeneticAlgorithm::OX(int *tour1, int *tour2, int size, int i, int j) {
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
        if(!find(tour1, i, j, tour2[k])) {
            int l=0;
            while(new_tour2[l]!=-1) l++;
            new_tour2[l] = tour2[k];
        }

        if(!find(tour2, i, j, tour1[k])) {
            int l=0;
            while(new_tour1[l]!=-1) l++;
            new_tour1[l] = tour1[k];
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

int GeneticAlgorithm::simulated_annealing(double finalTemp, double alpha, double stop_time, int neighbour_strategy) {
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
