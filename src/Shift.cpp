#include "Shift.h"

Shift::Shift(){
    //ctor
}

Shift::Shift(double cost, Driver* driver, Trailer* trailer) : cost_(cost), driver_(driver), trailer_(trailer){
    //ctor
}


Shift::~Shift(){
    stops_.clear();
}

void Shift::setStops(std::vector<Stop*> stops){
    stops_ = stops;
}
void Shift::setDriver(Driver* driver){
    driver_ = driver;
}
void Shift::setTrailer(Trailer* trailer){
    trailer_ = trailer;
}
void Shift::setCost(double cost){
    cost_ = cost;
}

std::vector<Stop*> Shift::getStop()  const{
    return stops_;
}

Driver* Shift::getDriver() const{
    return driver_;
}

Trailer* Shift::getTrailer() const{
    return trailer_;
}

double Shift::getCost() const{
    return cost_;
}
