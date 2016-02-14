#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <vector>

class Solution;

class GeneticAlgorithm
{
    public:
        GeneticAlgorithm();
        GeneticAlgorithm(int generations, int populationSize, double elitism, double cross, double mutate);
        virtual ~GeneticAlgorithm();

        inline void setGenerations(int generations){generations_ = generations;}
        inline void setPopulationSize(int populationSize){populationSize_ = populationSize;}
        inline void setElitism(int elitism){elitism_ = elitism;}
        inline void setCross(int cross){cross_ = cross;}
        inline void setMutate(int mutate){mutate_ = mutate;}

        inline int getGenerations(){return generations_;}
        inline int getPopulationSize(){return populationSize_;}
        inline double getElitism(){return elitism_;}
        inline double getCross(){return cross_;}
        inline double getMutate(){return mutate_;}
        inline std::vector<Solution*> getPopulation(){return population_;}

    protected:
    private:

    int generations_;
    int populationSize_;
    double elitism_;
    double cross_;
    double mutate_;
    std::vector<Solution*> population_;
};

#endif // GENETICALGORITHM_H
