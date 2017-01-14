//
// Created by Andrei Vasilev on 10/24/16.
//

#ifndef PHRASE_H
#define PHRASE_H

#include <vector>
#include <string>

class Phrase {

    friend class Population;

    private:
        int iPhraseLength;
        double dFitness;
        std::vector<char> vGenes;

    public:
        Phrase(){};
        Phrase(int);
        Phrase Procreate(Phrase);
        void SetFitness(std::string);
        void mutate(int);
        void PrintGenes();
};

#endif // PHRASE_H

