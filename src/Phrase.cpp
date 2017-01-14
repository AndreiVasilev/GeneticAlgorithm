//
// Created by Andrei Vasilev on 10/24/16.
//

#include "Phrase.h"
#include <cmath>
#include <random>
#include <iostream>

/*
    Initial Phrase Constructor. Populates a new Phrase vGene vector with random characters. Number of characters
    generated is equal to the length of the target phrase input by user. Initial fitness is set to 0.0
*/
Phrase::Phrase(int targetLength):
        iPhraseLength(targetLength), dFitness(0.0)
{
    std::random_device rd;
    std::uniform_int_distribution<int> cDist(32, 126);

    vGenes.reserve(iPhraseLength);
    for(int i = 0; i < iPhraseLength; ++i) {
        vGenes.push_back(static_cast<char>(cDist(rd)));
    }
}


/*
    Iterates through characters of a Phrase object and assigns a score based on how many characters
    are shared between the Phrase object and the Target phrase. The fitness is raised to a power so that objects
    with higher fitness are chosen later on for breeding at an exponentially higher rate instead of just a linear rate.
*/
void Phrase::SetFitness(std::string sTarget) {
    double fitnessScore = 0;

    for(int index = 0; index < iPhraseLength; index++) {
        if(vGenes[index] == sTarget[index])
            fitnessScore++;
    }

    dFitness = pow(fitnessScore / iPhraseLength, 20);

}


/*
    Creates a Child Phrase from two Parent Phrases by taking characters from each in an
    alternating fashion. Children Phrases will be the same length as the Parents.
*/
Phrase Phrase::Procreate(Phrase ParentB) {
    Phrase Child(iPhraseLength);
    Child.vGenes.clear();

    for(int index = 0; index < iPhraseLength; index++) {
        if(index == 0 || index % 2 == 0) {
            Child.vGenes.push_back(vGenes[index]);
        }
        else {
            Child.vGenes.push_back(ParentB.vGenes[index]);
        }
    }
    return Child;
}

/*
    Mutates a Phrase by inserting a random character at a random index within the Phrases
    vGenes vector. The chances of this happening is based on the mutation rate.
 */
void Phrase::mutate(int iMutationRate) {
    std::random_device rd;
    std::uniform_int_distribution<int> cDist(32, 126); // Distribution for char
    std::uniform_int_distribution<int> mDist(1, 100); // Distribution for randomRate
    std::uniform_int_distribution<int> iDist(0, iPhraseLength-1); // Distribution for index

    int randomRate = mDist(rd);

    if(randomRate <= iMutationRate) {
        int index = iDist(rd);
        char mutation = static_cast<char>(cDist(rd));
        vGenes[index] = mutation;
    }
}


// Prints the characters of the vGenes vector to the screen.
void Phrase::PrintGenes() {
    for(auto character : vGenes) {
        std::cout << character;
    }
    std::cout << std::endl;
}
