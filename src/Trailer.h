#ifndef TRAILER_H
#define TRAILER_H

#include "Driver.h"
#include "Location.h"

class Trailer
{
    public:
        Trailer();
        Trailer(int index, double capacity, double initialQuantity,
                    double distanceCost, std::vector<Driver*> drivers,
                    std::vector<Location*> locations);
        virtual ~Trailer();
        void setDistanceCost(double distanceCost);
        void setInitialQuantity(double initialQuantity);
        void setDrivers(std::vector<Driver*>drivers);
        void setLocations(std::vector<Location*>locations);
        void setCapacity(double capacity);

        std::vector<Driver*> getDrivers();
        std::vector<Location*> getLocations();
        double getDistanceCost();
        double getInicialQuantity();
        double getCapacity();
        int getIndex();
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
