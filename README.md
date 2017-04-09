# Phrase Evolution via Genetic Algorithm
This project is my first crack at object oriented programming. It is an implementation of a simple genetic algorithm that evolves a target phrase from complete randomness using the basic concepts of evolution: survival of the fittest and random mutation.  

<h2>The Algorithm In Action</h2>
<p align="left">
  <img src="https://s12.postimg.org/468ssxbql/ezgif_com_video_to_gif.gif"/>
</p>

<h2>How It Works</h2>
The algorithm essentially creates an artificial environment. One which contains organisms that are forced to evolve within it. In the real world, organisms must evolve to cope with the environments in which they live. The organisms that are best at this tend to survive more often, and in turn procreate more often. Consequently, the genes of the best suited organisms become more and more prevelant from generation to generation. This process continues until random genetic mutations occur that prove to be even more advantageous. Soon enough, these mutations become the new norm, the older genes are overrun by the "new and improved" and the cycle continues. 
<br><br>
As one can imagine, this type of algorithm can be implemented in many different ways. In this specific implementation, the "organisms" are character phrases and their "genes" are the characters that they each contain. The organisms with the "best" genes are the ones with the most characters equivalent to the characters of the target phrase that we are trying to evolve. When run, the algorithm creates a population of randomly generated phrases, each of which containing completely random characters. It then repeatedly creates new generations through procreation, introducing random mutations (new characters) every so often, until the target phrase is reached by a member in the population. 
<h2>Step 1: Input and Initial Creation</h2>
To begin, you must designate a target phrase that you are trying to evolve, a population size, and a mutation rate. All three of these variables greatly affect program run time. If you are attempting to evolve a very long phrase with a small population and a low mutation rate, it will undoubtedly take a very long time. Choosing a larger population can increase evolution speed, but only up to a certain point. Populations greater than 1000 or so can begin to actually slow the evolution process. Increasing mutation rate on the other hand, will almost always result in a faster evolution speed.
```sh
askInput(sTarget, PopSize, MutationRate);
Population Population1(sTarget, PopSize, MutationRate);
```
Once the target phrase, population size, and mutation rate have been assigned, the initial population object is created. It's a std::vector of Phrase objects and is populated with the designated "population size" number of Phrases. The one constraint is that all Phrase objects are and always will be the same length as the target phrase. 
```sh
Population::Population(std::string Target, int PopSize,  int MutationRate):
        sTarget(Target), iPopulationSize(PopSize),
        iMutationRate(MutationRate), iTargetLength(Target.size())
{
    vPopulation.reserve(iPopulationSize);
    vOffspring.reserve(iPopulationSize);
            
    for(int i = 1; i <= iPopulationSize; i++) {
        vPopulation.push_back(Phrase(iTargetLength));
    }
}
```
When each Phrase object is created, its genes (a std::vector) are populated with randomly generated ASCII characters using a random number device from the \<random> header. In fact, all random number generation in this algorithm is done by using random number devices. 
```sh
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
```
<h2>Step 2: Assigning Fitness Scores</h2>
Once the intial population has been generated, the evolution process can begin. This process contains 2 main steps and will continue to carry them out until the target phrase has been reached by a member of the population. The first step consists of checking the entire population and assigning "fitness" scores to each Phrase object. 
```sh
while (!Population1.PerfectScore()) {
    Population1.AssignFitness();
    Population1.NaturalSelection();
    Population1.ReplacePopulation();
    Generation++;
}
```
Fitness scores are determined by the AssignFitness() method, which checks how many equivalent characters are shared between a Phrase object and the target phrase. The characters must be in the proper location to count towards fitness. Throughout the program's runtime, this function also calls the PrintGenes() method for all Phrase objects that have a significatly high fitness rating. The Phrase's genes are then printed to STDOUT. This is what allows us to see the evolution of the phrases.
```sh
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
```
While iterating through the population, the SetFitness() method is called, which compares characters between the Phrase and the target and then assigns a score based on how many match. Once a score is assigned, it is exponentially adjusted. This makes it so that all fitness scores are mapped to an exponential curve. In turn, the Phrases with higher fitness scores are chosen at a exponentially higher rate when it comes time to procreate. 
```sh
void Phrase::SetFitness(std::string sTarget) {
    double fitnessScore = 0;

    for(int index = 0; index < iPhraseLength; index++) {
        if(vGenes[index] == sTarget[index])
            fitnessScore++;
    }

    dFitness = pow(fitnessScore / iPhraseLength, 20);
}
```
<h2>Step 3: Natural Selection</h2>
Once all Phrase objects have been assigned a fitness score, the natural selection process begins with the method call NaturalSelection(). During this process, pairs of Phrase objects are chosen as "parents" to create new child Phrase objects. The number of children created is equal to the size of the original population, and is later swapped out with the old population. This prevents the population from growing out of control and potentially crashing the program.
<br><br>
Along with this, for each child created the algorithm is prevented from chosing the same parent Phrase twice. Meaning that every child Phrase object has two unique parents.
```sh
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
```
Parent Phrases are grabbed from the population at random using the GrabPhrase() method. The chance of any one parent being chosen is correlated to their fitness score. A random number of type double between 0 and the highest current fitness score is generated and if that number falls below the currently selected Phrase's fitness score, then the Phrase is chosen as a parent. 
<br><br>
For example, if the highest fitness score is currently 0.7, and a Phrase has a fitness score of 0.1, the likelihood that the randomly generated number between 0.0 and 0.7 will fall below 0.1 is fairly low. However, if the Phrase were to have a score of 0.65, the likelihood would be quite high. Thus, Phrases with higher scores are chosen more often.
```sh
Phrase Population::GrabPhrase() {
    std::random_device rd;
    std::uniform_int_distribution<int> dist1(0, iPopulationSize-1);
    std::uniform_real_distribution<double> dist2(0.0, dHighestFitness);

    while(true) {
        int rIndex = dist1(rd);
        double rFitness = dist2(rd);:

        Phrase Parent = vPopulation[rIndex];
        if (rFitness < Parent.dFitness)
            return Parent;
    }
}
```
Once two unique parent Phrase objects have been selected, a child object is created using the Procreate() method. This function takes the genes from each parent, and splices them together in a zipper fashion to create the child Phrase's genes.
```sh
Phrase Phrase::Procreate(Phrase ParentB) {
    Phrase Child;
    Child.iPhraseLength = iPhraseLength;
    Child.vGenes.reserve(iPhraseLength);

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
```
Once a child Phrase object is created, the MutateChild() method is called. It uses similar logic to the GrabPhrase() method. A random number between 0 and 100 is generated and if it's below the mutation rate number originally set at the beginning of the program, the function replaces a character in the Child Phrase's genes with a random ASCII character. The character that is replaced is also randomly chosen. 
```sh
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
```
Once the child population has been filled to the size of the original population, the child population is swapped with the parent population, the parent population is deleted, and the entire process starts over again.
```sh
void Population::ReplacePopulation() {
    vPopulation.swap(vOffspring);
    vOffspring.clear();
}
```
<h2>Step 4: Final Evolution</h2>
Once the algorithm has generated a sufficient number of generations, the target phrase will eventually be reached by a member of the population. How long this takes depends on the complexity and length of the phrase, the population size, and the mutation rate. After each new generation is created, the highest fitness score is checked. If the highest fitness score is equal to 1.0, that means that the target has been reached.
```sh
while (!Population1.PerfectScore()) {
    Population1.AssignFitness();
    Population1.NaturalSelection();
    Population1.ReplacePopulation();
    Generation++;
}
```
```sh
bool Population::PerfectScore() {
    if (dHighestFitness == dPerfectScore)
        return true;
    else
        return false;
}
```
<h2>Conclusion</h2>
This type of algorithm can be applied to various differnet situations, and this implementation is probably the simplest. The conditions that govern fitness in the real world can be very complex, whereas in this situation they are very simple. Nevertheless, I find the algorithm extremely interesting. I plan on studying it further and hopefully implementing it in more complicated ways in the future.


