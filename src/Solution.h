#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

class Shift;

class Solution
{
    public:
        Solution();
        Solution(double cost, std::vector<Shift*> shifts,
                double infeasibilityCost);
        virtual ~Solution();
    protected:
    private:
        double cost_;
        std::vector<Shift*> shifts_;
        double infeasibilityCost_;//inviabilidade

};

#endif // SOLUTION_H
