#include "Solution.h"

Solution::Solution()
{
    //ctor
}

Solution::Solution(double cost, std::vector<Shift*> shifts,
                double infeasibilityCost):
                cost_(cost),
                shifts_(shifts),
                infeasibilityCost_(infeasibilityCost){

}

Solution::~Solution()
{
    //dtor
    shifts_.clear();
}
