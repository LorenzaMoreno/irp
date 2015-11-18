#include "InputData.h"

/*Modificação 18/11 por Gisele*/
InputData::InputData(){
    //ctor
}

InputData::InputData(std::vector<Driver*> &drivers, std::vector<Trailer*> &trailers, std::vector<Location*> &location, std::vector<Base*> &bases, std::vector<Customer*> &customers, double** distance, int** time, int** windowSize):
                        drivers_(drivers), trailers_(trailers), location_(location), bases_(bases), customers_(customers), distance_(distance), time_(time), windowSize_(windowSize) {
    //ctor
}

InputData::~InputData(){
    drivers_.clear();
    trailers_.clear();
    location_.clear();
    bases_.clear();
    customers_.clear();
    //desalocar as matrizes!!!
    //dtor
}

std::vector<Driver*> InputData::getDrivers() const{
    return drivers_;
}
std::vector<Trailer*> InputData::getTrailers() const{
    return trailers_;
}
std::vector<Location*> InputData::getLocation() const{
    return location_;
}
std::vector<Base*> InputData::getBases() const{
    return bases_;
}
std::vector<Customer*> InputData::getCustomers() const{
    return customers_;
}

void InputData::setDrivers(std::vector<Driver*> drivers){
    drivers_ = drivers;
}

void InputData::setTrailers(std::vector<Trailer*> trailers){
   trailers_ = trailers;
}

void InputData::setLocation(std::vector<Location*> location){
    location_ = location;
}

void InputData::setBases(std::vector<Base*> bases){
    bases_ = bases;
}

void InputData::setCustomers( std::vector<Customer*> customers){
    customers_ = customers;
}
