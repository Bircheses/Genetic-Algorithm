#include "Utilities/StaticFunctions.cpp"
#include "GeneticAlgorithm.h"

// Main function
int main() {
    GeneticAlgorithm GA;

    int t1[9] = {1,2,3,4,5,6,7,8,9};
    int t2[9] = {9,3,7,8,2,6,5,1,4};

    GA.OX(t1, t2, 9, 3, 7);

    //GA.genetic_algorithm(10000, 0, 0, 10, 0.1, 0.8);
    cin.get();
    return 0;
}