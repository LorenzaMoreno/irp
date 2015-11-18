#ifndef TRAILER_H
#define TRAILER_H

#include "Driver.h"
#include "Location.h"

class Trailer
{
    public:
        Trailer();
        Trailer(int index, double capacity, double initialQuantity, double distanceCost);
        Trailer(int index, double capacity, double initialQuantity, double distanceCost, std::vector<Driver*> &drivers, std::vector<Location*> &locations);
        virtual ~Trailer();

        std::vector<Driver*> getDrivers() const;
        std::vector<Location*> getLocations() const;
        double getDistanceCost() const;
        double getInicialQuantity() const;
        double getCapacity() const;
        int getIndex() const;

        void setIndex(int);
        void setDistanceCost(double);
        void setInitialQuantity(double);
        void setDrivers(std::vector<Driver*>);
        void setLocations(std::vector<Location*>);
        void setCapacity(double);
    protected:
    private:
        int index_;
        double capacity_;
        double initialQuantity_;
        double distanceCost_;
        std::vector<Driver*> drivers_;
        std::vector<Location*> locations_;
};

#endif // TRAILER_H
