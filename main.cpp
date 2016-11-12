//
// Created by Andrei Vasilev on 10/24/16.
//

#include <iostream>
#include <time.h>
#include <string>
#include <iomanip>
#include "Population.h"

void askInput(std::string &sTarget, int &PopSize, int &MutationRate);
void runProgram();

int main() {

    runProgram();

}

// Loops the evolution process until a perfect fitness score of 1.0 is achieved.
void runProgram() {
    clock_t programStart,programEnd;
    programStart=clock();

    std::string sTarget;
    int PopSize, MutationRate, Generation = 1;

    askInput(sTarget, PopSize, MutationRate);
    Population Population1(sTarget, PopSize, MutationRate);

    while (!Population1.PerfectScore()) {
        Population1.AssignFitness();
        Population1.NaturalSelection();
        Population1.ReplacePopulation();
        Generation++;
    }

    std::cout << "\nTarget Reached: " << sTarget << std::endl;
    std::cout << "Generation Count: " << Generation << std::endl;

    programEnd=clock();
    float diff (static_cast<float>(programEnd)-static_cast<float>(programStart));
    float seconds = diff / CLOCKS_PER_SEC;
    std::cout<< "Evolution runtime: " << std::setprecision(2) << seconds << " seconds" << std::endl;
}

void askInput(std::string &sTarget, int &PopSize, int &MutationRate) {

    std::cout << "Please enter the target phrase to be evolved: " << std::endl;
    getline(std::cin, sTarget);

    std::cout << "\nPlease enter population size: " << std::endl;
    std::cin >> PopSize;

    std::cout << "\nPlease enter a mutation rate percentage: " << std::endl;
    std::cin >> MutationRate;

    std::cout << std::endl;
}