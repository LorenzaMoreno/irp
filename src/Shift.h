#ifndef SHIFT_H
#define SHIFT_H

#include <vector>
#include <string>

#include "Driver.h"
#include "Trailer.h"
#include "Stop.h"
#include "InputData.h"
#include "Penalties.h"
#include "Formatter.h"
#include "Location.h"

class Solution;
class Shift{
    public:
        Shift();
        Shift(double cost, Driver* driver, Trailer* trailer);
        virtual ~Shift();

        operator std::string(){
            std::stringstream stream;
            stream << "Driver: " << driver_->getIndex() << std::endl;
            stream << "Trailer: " << trailer_->getIndex() << std::endl;
            stream << "Initial Instant: " << initialInstant_ << std::endl;
            stream << "Final Instant: " << finalInstant_ << std::endl;
            return stream.str();
        }

        inline std::vector<Stop*>* getStop(){return &stops_;}
        inline Driver* getDriver(){return driver_;}
        inline Trailer* getTrailer(){return trailer_;}
        inline double getCost(){return cost_;}
        inline Solution* getSolution(){return solution_;}
        inline double getInitialInstant(){return initialInstant_;}
        inline double getFinalInstant(){return finalInstant_;}
        inline double getInitialLoad(){return initialLoad_;}
        inline double getRemnantLoad(){return remnantLoad_;}

        inline void setStops(std::vector<Stop*> stops){stops_ = stops;}
        inline void setDriver(Driver* driver){driver_ = driver;}
        inline void setTrailer(Trailer* trailer){trailer_ = trailer;}
        inline void setCost(double cost){cost_ = cost;}
        inline void setSolution(Solution* solution){solution_ = solution;}
        inline void setInitialInstant(int initialInstant){initialInstant_ = initialInstant;}
        inline void setFinalInstant(int finalInstant){finalInstant_ =finalInstant;}
        inline void setInitialLoad(int initialLoad){initialLoad_ =initialLoad;}
        inline void setRemnantLoad(int remnantLoad){remnantLoad_ =remnantLoad;}
        std::string toString(std::string tab="   ") const;

        double calcCost(bool print = false);
        double getQuantityDelivered();

    protected:
    private:

        void toString_appendStop(char* data,int stopIndex) const;

        std::vector<Stop*> stops_;
        double cost_;
        Driver* driver_;
        Trailer* trailer_;
        Solution* solution_;
        double initialInstant_, finalInstant_;
        double initialLoad_, remnantLoad_;
};

#endif // SHIFT_H
