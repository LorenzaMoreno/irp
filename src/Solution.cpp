#include "Solution.h"

Solution::Solution()
{
    //ctor
}

Solution::Solution(double cost, std::vector<Shift*> &shifts, double infeasibilityCost):
                cost_(cost), shifts_(shifts), infeasibilityCost_(infeasibilityCost){
}

Solution::~Solution()
{
    shifts_.clear();
}

double Solution::getCost() const{
    return cost_;
}

void Solution::setCost(double cost){
    cost_ = cost;
}

std::vector<Shift*> Solution::getShift() const{
    return shifts_;
}

void Solution::setShift(std::vector<Shift*> shifts){
    shifts_ = shifts;
}

double Solution::getInfreasivilityCost() const{
    return infeasibilityCost_;
}

void Solution::setInfreasibilityCost(double infreasibilityCost){
    infeasibilityCost_ = infreasibilityCost;
}
