#include "Utilities/StaticFunctions.cpp"
#include "GeneticAlgorithm.h"

// Main function
int main() {
    // read_conf_file();

    int tab1[9] = {1,2,3,4,5,6,7,8,9};
    int tab2[9] = {4,5,2,1,8,7,6,9,3};

    GeneticAlgorithm GA;
    auto [f, s] = GA.PMX(tab1, tab2, 9, 3, 7);

    /*show_tour(f, 10);
    show_tour(s, 10);*/

    delete [] f;
    delete [] s;

    cin.get();

    return 0;
}