#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <map>

#include "Customer.h"
#include "Stop.h"
#include "Shift.h"
#include "InputData.h"

class Shift;

class Solution
{
    public:
        Solution();
        Solution(double cost, double infeasibilityCost);
        virtual ~Solution();

        inline double getCost() const{return cost_;}
        inline std::vector<std::vector<Shift*> >* getDriverInst(){return &driverInst_;}
        inline std::vector<std::vector<Shift*> >* getTrailerInst(){return &trailerInst_;}
        inline std::vector<std::vector<double> >* getstockLevelInst_(){return &stockLevelInst_;}
        inline std::multimap<int, Customer*>* getCustomer(){return &customer_;}
        inline double getInfeasibilityCost() const{return infeasibilityCost_;}

        void reset();
        void clear();

        void insertShift(Shift* shift);
        void removeShift(Shift* shift);

        void insertStopInShift(Shift* shift, Stop* stop);
        void removeStopFromShift(Shift* shift, Stop* stop);

        void updateCost(/*Penalties penalties*/);

    protected:
    private:
        double cost_;
        std::vector<std::vector<Shift*> > driverInst_; //linha = driver, coluna = instante por hora
        std::vector<std::vector<Shift*> > trailerInst_; //linha = trailer, coluna = instante por hora
        std::vector<std::vector<double> > stockLevelInst_; //linha = customer, coluna = instante por horas, nível do estoque
        std::multimap<int, Customer*> customer_;
        double infeasibilityCost_;//inviabilidade

};

#endif // SOLUTION_H
