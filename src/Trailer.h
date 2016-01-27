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

        inline std::vector<Driver*>* getDrivers(){return &drivers_;}
        inline std::vector<Location*>* getLocations(){return &locations_;}
        inline double getDistanceCost() const{return distanceCost_;}
        inline double getInicialQuantity() const{return initialQuantity_;}
        inline double getCapacity() const{return capacity_;}
        inline int getIndex() const{return index_;}
        std::string toString() const;

        inline void setIndex(int index){index_ = index;}
        inline void setDistanceCost(double distanceCost){distanceCost_ = distanceCost;}
        inline void setInitialQuantity(double initialQuantity){initialQuantity_ = initialQuantity;}
        inline void setCapacity(double capacity){capacity_ = capacity;}
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
