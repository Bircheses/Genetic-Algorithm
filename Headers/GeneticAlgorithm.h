#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "../Utilities/Structures.cpp"

class GeneticAlgorithm {
private:
    const int maxStagnation = 1000;
    int** matrix = nullptr;
    int size = 9;
    static int* copy(const int* tour, int size);
    static int calculate_cost(int** matrix, int* tour, int size);
    static void swap(int *tour, int i, int j);
    static void insert(int *tour, int i, int j);
    static void inverse(int *tour, int i, int j);
    static int* generate_random_tour(int size);
    static bool find(int *&tour, int begin, int end, int value);
    static int find_index(const int* arr, int begin, int end, int value);
    void OX(int* &tour1, int* &tour2, int begin, int end) const;
    int tournament_selection(int** &tour_array, int tournament_size, int population_size);
public:
    void load_matrix(int** matrix, int size);
    void PMX(int* &tour1, int* &tour2, int begin, int end) const;
    return_values genetic_algorithm(double stop_time, int mutation_strategy, int crossing_strategy, int population_size, double wsp_mut, double wsp_cros, int tournament_size=3, double new_gen_intake=0.9);
    ~GeneticAlgorithm();
};



#endif