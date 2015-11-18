#include "Trailer.h"

Trailer::Trailer()
{
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost){
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost, std::vector<Driver*> &drivers, std::vector<Location*> &locations):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost), drivers_(drivers), locations_(locations){
    //ctor
}

Trailer::~Trailer()
{
    locations_.clear();
    drivers_.clear();
}

int Trailer::getIndex() const{
    return index_;
}

void Trailer::setIndex(int index){
    index_ = index;
}
double Trailer::getCapacity() const{
    return capacity_;
}

void Trailer::setCapacity(double capacity){
    capacity_ = capacity;
}

double Trailer::getInicialQuantity() const{
    return initialQuantity_;
}

void Trailer::setInitialQuantity(double initialQualtity){
    initialQuantity_ = initialQualtity;
}

double Trailer::getDistanceCost() const{
    return distanceCost_;
}

void Trailer::setDistanceCost(double distanceCost){
    distanceCost_ = distanceCost;
}
