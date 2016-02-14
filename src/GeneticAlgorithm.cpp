#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int generations, int populationSize, double elitism, double cross, double mutate)
{
    setGenerations(generations);
    setPopulationSize(populationSize);
    setElitism(elitism);
    setCross(cross);
    setMutate(mutate);

    while(getPopulation().size() < getPopulationSize()){
//        getPopulation().push_back(new Solution*);
    }
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    //dtor
}
