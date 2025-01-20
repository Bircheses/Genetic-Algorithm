#include "../Headers/GeneticAlgorithm.h"

#include <algorithm>

#include "../Headers/Counter.h"
#include "../Utilities/StaticFunctions.cpp"
#include <ctime>
#include <cmath>
#include <random>

#include "NearestNeighbourAlgorithm.h"


bool GeneticAlgorithm::find(int *&tour, int begin, int end, int value) {
    for(int i=begin; i<end; i++) {
        if(tour[i] == value) {
            return true;
        }
    }
    return false;
}

void GeneticAlgorithm::OX(int *&tour1, int *&tour2, int begin, int end) const {
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

    delete [] tour1;
    delete [] tour2;

    tour1 = new_tour1;
    tour2 = new_tour2;
}

void GeneticAlgorithm::PMX(int *&tour1, int *&tour2, int begin, int end) const {
    int* new_tour1 = new int[size];
    int* new_tour2 = new int[size];

    for(int k=0; k<size; k++) {
        new_tour1[k] = -1;
        new_tour2[k] = -1;
    }

    for(int k=begin; k<end; k++) {
        new_tour1[k] = tour2[k];
        new_tour2[k] = tour1[k];
    }

    for(int k=0; k<size; k++) {
        if(k>=begin && k<end) continue;

        if(!find(tour1, begin, end, tour2[k])) {
            new_tour2[k] = tour2[k];
        }

        if(!find(tour2, begin, end, tour1[k])) {
            new_tour1[k] = tour1[k];
        }
    }

    for(int k=0; k<size; k++) {
        if(k>=begin && k<end) continue;

        if(!find(new_tour1, 0, size, tour2[k])) {
            int l=0;
            while(new_tour1[l]!=-1){
                l++;
            }
            new_tour1[l]=tour2[k];
        }

        if(!find(new_tour2, 0, size, tour1[k])) {
            int l=0;
            while(new_tour2[l]!=-1){
                l++;
            }
            new_tour2[l] = tour1[k];
        }
    }

    delete [] tour1;
    delete [] tour2;

    tour1 = new_tour1;
    tour2 = new_tour2;
}

int GeneticAlgorithm::tournament_selection(int **&tour_array, int tournament_size, int population_size) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);// CLOCK_MONOTONIC gwarantuje stały wzrost czasu
    srand(ts.tv_nsec ^ ts.tv_sec);
    int bestCost = INT_MAX, bestIndex=0;
    for(int i=0; i<tournament_size; i++){
        int index = rand() % population_size;
        int cost = calculate_cost(matrix, tour_array[index], size);
        if(cost < bestCost){
            bestIndex = index;
        }
    }
    return bestIndex;
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

return_values GeneticAlgorithm::genetic_algorithm(double stop_time, int mutation_strategy, int crossing_strategy, int population_size, double wsp_mut, double wsp_cros, int tournament_size, double new_gen_intake) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);// CLOCK_MONOTONIC gwarantuje stały wzrost czasu
    srand(ts.tv_nsec ^ ts.tv_sec);

    NearestNeighbourAlgorithm NNA;
    NNA.load_matrix(matrix, size);

    Counter counter;
    counter.start();

    int bestCost = INT32_MAX;
    int* bestTour = new int[size];

    double time_found = 0;
    int ** old_gen = new int*[population_size];
    for(int i=0; i<population_size; i++){
        old_gen[i] = NNA.find_shortest_path();
        //old_gen[i] = generate_random_tour(size);
    }

    int ** new_gen = new int*[population_size];

    while(counter.getElapsedTime() < stop_time){
        for(int i=0; i<population_size; i+=2) {

            //Generating two random parent indexes using tournament selection
            int index1 = tournament_selection(old_gen, tournament_size, population_size);
            int index2 = tournament_selection(old_gen, tournament_size, population_size);

            int* tour1 = copy(old_gen[index1], size);
            int* tour2 = copy(old_gen[index2], size);

            //Crossing strategy with a propability given by user
            if (wsp_cros > (rand() / (double) RAND_MAX)) {
                int begin = rand() % size;
                int end = rand() % size;
                while (begin == end) end = rand() % size;

                if(begin > end) {
                    int temp = end;
                    end = begin;
                    begin = temp;
                }
                switch (crossing_strategy) {
                    //PMX
                    case 0: {
                        PMX(tour1, tour2, begin, end);
                        break;
                    }
                    //OX
                    case 1: {
                        OX(tour1, tour2, begin, end);
                        break;
                    }
                    default:
                        break;
                }
            }

            //Mutation strategy with a propability given by user
            if (wsp_mut > (rand() / (double) RAND_MAX)) {
                int begin = rand() % size;
                int end = rand() % size;
                while (begin == end) end = rand() % size;

                if(begin > end) {
                    int temp = end;
                    end = begin;
                    begin = temp;
                }
                switch (mutation_strategy) {
                    //Swap
                    case 0: {
                        swap(tour1, begin, end);
                        swap(tour2, begin, end);
                        break;
                    }
                    //Insert
                    case 1: {
                        insert(tour1, begin, end);
                        insert(tour2, begin, end);
                        break;
                    }
                    default:
                        break;
                }
            }

            //Filling new_gen with acquired tours
            new_gen[i] = tour1;
            new_gen[i+1] = tour2;
        }

        //Sorting the old_gen to take only best (1-new_gen_intake) of old_gen to new_gen
        sort(old_gen, old_gen + population_size, [&](int* first, int* second) -> bool {
            return calculate_cost(matrix, first, size) < calculate_cost(matrix, second, size);
        });

        //Save the best cost of old_gen
        int tempCost = calculate_cost(matrix, old_gen[0], size);
        if(tempCost < bestCost) {
            time_found = counter.getElapsedTime();
            bestCost = tempCost;
            if(bestTour != nullptr) delete [] bestTour;
            bestTour = copy(old_gen[0], size);
        }

        //Randomly shuffle the new_gen so we take random new_gen_intake of new_gen
        shuffle(new_gen, new_gen + population_size, std::mt19937(std::random_device()()));

        int last_index = static_cast<int>(new_gen_intake * population_size);

        for(int i=0, j=last_index; j<population_size; i++, j++) {
            delete new_gen[j];
            new_gen[j] = copy(old_gen[i], size);
        }

        //New generation becomes old generation
        //Prepare the space by deleting the old generation
        for(int i=0; i<population_size; i++){
            delete [] old_gen[i];
        }
        delete [] old_gen;

        //Initialize old_gen again
        old_gen = new int*[population_size];

        //Copy values from new gen to old gen
        for(int i=0; i<population_size; i++){
            old_gen[i] = copy(new_gen[i], size);
        }

        //Delete new_gen after not needing it for anything more
        for(int i=0; i<population_size; i++){
            delete [] new_gen[i];
        }
        delete [] new_gen;

        //Initialize new_gen again
        new_gen = new int*[population_size];
    }

    //Clean up the memory after the program
    for(int i=0; i<population_size; i++){
        delete [] old_gen[i];
    }
    delete [] old_gen;
    delete [] new_gen;

    return_values rv = {bestCost, bestTour, time_found};

    counter.stop();

    return rv;
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
