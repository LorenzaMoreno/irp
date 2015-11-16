#ifndef SHIFT_H
#define SHIFT_H

#include "Driver.h"
#include "Trailer.h"

class Stop{
    public:
        Stop();
        Stop(double quantity, int arriveTime, Location* location):
        quantity_(quantity), arriveTime_(arriveTime), location_(location){

        };
        virtual ~Stop();


    private:
        Location* location_;
        int arriveTime_;
        double quantity_;



};

class Shift
{
    public:
        Shift();
        Shift(double cost, Driver* driver, Trailer* trailer);
        virtual ~Shift();
        std::vector<Stop*> getStops();
        void setStop(std::vector<Stop*> stops);
        void setDriver(Driver* driver);
        void setTrailer(Trailer* trailer);
        void setCost(double cost);



    protected:
    private:
        std::vector<Stop*> stops_;
        double cost_;
        Driver* driver_;
        Trailer* trailer_;
};

#endif // SHIFT_H
