#include "CurrentSolution.h"

CurrentSolution::CurrentSolution()
{
    //ctor
}

CurrentSolution::CurrentSolution(double cost, std::vector<Shift*> &shifts, double infeasibilityCost): Solution(cost, shifts, infeasibilityCost){
}

CurrentSolution::~CurrentSolution()
{
    //dtor
}
