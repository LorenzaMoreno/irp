#ifndef CURRENTSOLUTION_H
#define CURRENTSOLUTION_H

#include "Solution.h"

class CurrentSolution : public Solution
{
    public:
        CurrentSolution();
        CurrentSolution(double cost, std::vector<Shift*> &shifts, double infeasibilityCost);
        virtual ~CurrentSolution();
    protected:
    private:
};

#endif // CURRENTSOLUTION_H
