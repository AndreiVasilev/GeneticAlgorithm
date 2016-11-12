//
// Created by Andrei Vasilev on 10/24/16.
//

#include "Population.h"
#include <random>
#include <iostream>


// Initial Population constructor. Fills population object vector with new Phrase
// objects. Number of Phrases generated is equal to the population size.
Population::Population(std::string Target, int PopSize,  int MutationRate):
    sTarget(Target), iPopulationSize(PopSize),
    iMutationRate(MutationRate), iTargetLength(Target.size())
{
    for(int i = 1; i <= iPopulationSize; i++) {
        vPopulation.push_back(Phrase(iTargetLength));
    }
}


// Sets the fitness score for all Phrase objects in the Population, records the highest fitness,
// and prints the Phrase with the highest fitness.
void Population::AssignFitness() {
    for(auto item = vPopulation.begin(); item != vPopulation.end(); item++) {

        (*item).SetFitness(sTarget);

        if((*item).dFitness >= dHighestFitness) {
            dHighestFitness = (*item).dFitness;
        }

        if((*item).dFitness > dHighestFitness/2) {
            (*item).PrintGenes();
        }
    }
}


// Grabs Phrases from the Population based on their fitness score. Phrases with
// a higher fitness scores will be chosen at proportionally higher rate.
Phrase Population::GrabPhrase() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist1(0, iPopulationSize-1);
    std::uniform_real_distribution<double> dist2(0.0, dHighestFitness);

    while(true) {
        int rIndex = dist1(mt);
        double rFitness = dist2(mt);

        Phrase Parent = vPopulation[rIndex];
        if (rFitness < Parent.dFitness)
            return Parent;
    }
}


// Populates vOffspring vector with children Phrases.
void Population::NaturalSelection() {

    for(int i = 1; i <= iPopulationSize; i++) {
        Phrase ParentA = GrabPhrase();
        Phrase ParentB = GrabPhrase();

        while(ParentA.vGenes == ParentB.vGenes) {
            ParentB = GrabPhrase();
        }

        Phrase Child = ParentA.Procreate(ParentB);
        Child.mutate(iMutationRate);

        vOffspring.push_back(Child);
    }
}


// Replaces vPopulation vector with contents of vOffspring vector
void Population::ReplacePopulation() {
    vPopulation.clear();

    for(int i = 0; i <= iPopulationSize - 1; i++) {
        vPopulation.push_back(vOffspring[i]);
    }

    vOffspring.clear();
}

// Checks if a perfect fitness score has been achieved.
bool Population::PerfectScore() {
    if (dHighestFitness == dPerfectScore)
        return true;
    else
        return false;
}

