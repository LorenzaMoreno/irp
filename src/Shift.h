#ifndef SHIFT_H
#define SHIFT_H

#include "Driver.h"
#include "Trailer.h"
#include "Stop.h"
#include "InputData.h"
#include "Penalties.h"
#include "Formatter.h"

template<typename Base, typename T>
inline bool instanceof(const T *ptr) {
    return dynamic_cast<const Base*>(ptr) != nullptr;
}

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
        inline int getInitialInstant(){return initialInstant_;}
        inline int getFinalInstant(){return finalInstant_;}

        inline void setStops(std::vector<Stop*> stops){stops_ = stops;}
        inline void setDriver(Driver* driver){driver_ = driver;}
        inline void setTrailer(Trailer* trailer){trailer_ = trailer;}
        inline void setCost(double cost){cost_ = cost;}
        inline void setSolution(Solution* solution){solution_ = solution;}
        inline void setInitialInstant(int initialInstant){initialInstant_ = initialInstant;}
        inline void setFinalInstant(int finalInstant){finalInstant_ =finalInstant;}

        void calcCost();
        double getQuantityDelivered();

    protected:
    private:
        std::vector<Stop*> stops_;
        double cost_;
        Driver* driver_;
        Trailer* trailer_;
        Solution* solution_;
        double initialInstant_, finalInstant_;
        double initialLoad_, remnantLoad;
};

#endif // SHIFT_H
