#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "../Utilities/Structures.cpp"

class GeneticAlgorithm {
private:
    const int maxStagnation = 1000;
    int** matrix = nullptr;
    int size = 0;
    int* copy(const int* tour, int size);
    int calculate_cost(int** matrix, int* tour, int size);
    static void swap(int *tour, int i, int j);
    static void insert(int *tour, int i, int j);
    static void inverse(int *tour, int i, int j);
    int* generate_random_tour(int size);
    bool find(int *tour, int begin, int end, int value);
    double_tour PMX(int* tour1, int* tour2, int size, int i, int j);
    double_tour OX(int* tour1, int* tour2, int size, int i, int j);
public:
    void load_matrix(int** matrix, int size);
    int simulated_annealing(double finalTemp, double alpha, double stop_time, int neighbour_strategy);
    ~GeneticAlgorithm();
};



#endif