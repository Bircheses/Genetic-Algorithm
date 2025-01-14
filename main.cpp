#include "Utilities/StaticFunctions.cpp"
#include "GeneticAlgorithm.h"

// Main function
int main() {
    GeneticAlgorithm GA;

    GA.genetic_algorithm(10000, 0, 0, 10, 0.1, 0.8);
    cin.get();
    return 0;
}