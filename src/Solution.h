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
        inline std::vector<std::vector<std::vector<Shift*> > > * getDriverInst(){return &driverInst_;}
        inline std::vector<std::vector<std::vector<Shift*> > > * getTrailerInst(){return &trailerInst_;}
        inline std::vector<std::vector<double> >* getstockLevelInst_(){return &stockLevelInst_;}
        inline std::vector<std::vector<std::vector<Stop*> > > * getLocationInstStop() {return &locationInstStop_;}
        inline std::multimap<int, Customer*>* getSafetyLevelInst(){return &safetyLevelInst_;}
        inline double getInfeasibilityCost() const{return infeasibilityCost_;}

        void reset();
        void clear();

        void insertShift(Shift* shift);
        void removeShift(Shift* shift);

        void insertStopInShift(Shift* shift, Stop* stop);
        void removeStopFromShift(Shift* shift, Stop* stop);

        bool checkShift(Shift*);
        bool checkStop(Stop*);

        void updateCost(/*Penalties penalties*/);
        void calcCost();

    protected:
    private:
        double cost_;

        //line = driver, column = instant by hour, column depth = shifts organized by instant
        std::vector<std::vector<std::vector<Shift*> > > driverInst_;

        //line = trailer, column = instant by hour, column depth = shift organized by instant
        std::vector<std::vector<std::vector<Shift*> > > trailerInst_;

         //line = customer, column = instant by hour, stock level
        std::vector<std::vector<double> > stockLevelInst_;

         //line = location, column = instant by hour, column depth = stops organized by instant
        std::vector<std::vector<std::vector<Stop*> > > locationInstStop_;

         //instant where the stock reaches the safety level
        std::multimap<int, Customer*> safetyLevelInst_;

        double infeasibilityCost_;

};

#endif // SOLUTION_H
