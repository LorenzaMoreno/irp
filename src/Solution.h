#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>

class Shift;

class Solution
{
    public:
        Solution();
        Solution(double cost, std::vector<Shift*> &shifts, double infeasibilityCost);
        virtual ~Solution();

        double getCost() const;
        std::vector<Shift*> getShift() const;
        double getInfreasivilityCost() const;

        void setCost(double);
        void setShift(std::vector<Shift*>);
        void setInfreasibilityCost(double);

    protected:
    private:
        double cost_;
        std::vector<Shift*> shifts_;
        double infeasibilityCost_;//inviabilidade

};

#endif // SOLUTION_H
