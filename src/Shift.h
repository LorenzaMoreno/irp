#ifndef SHIFT_H
#define SHIFT_H

#include "Driver.h"
#include "Trailer.h"
#include "Stop.h"

class Shift
{
    public:
        Shift();
        Shift(double cost, Driver* driver, Trailer* trailer);
        virtual ~Shift();

        std::vector<Stop*> getStop() const;
        Driver* getDriver() const;
        Trailer* getTrailer() const;
        double getCost() const;

        void setStops(std::vector<Stop*>);
        void setDriver(Driver*);
        void setTrailer(Trailer*);
        void setCost(double);

    protected:
    private:
        std::vector<Stop*> stops_;
        double cost_;
        Driver* driver_;
        Trailer* trailer_;
};

#endif // SHIFT_H
