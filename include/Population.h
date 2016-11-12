//
// Created by Andrei Vasilev on 10/24/16.
//

#ifndef POPULATION_H
#define POPULATION_H

#include "Phrase.h"
#include <vector>

class Population {
private:
    const double dPerfectScore = 1.0;
    int iPopulationSize, iTargetLength, iMutationRate;
    double dHighestFitness;
    std::string sTarget;
    std::vector<Phrase> vPopulation;
    std::vector<Phrase> vOffspring;

public:
    Population(std::string, int, int);
    void AssignFitness();
    Phrase GrabPhrase();
    void NaturalSelection();
    void ReplacePopulation();
    bool PerfectScore();
};


#endif //POPULATION_H
