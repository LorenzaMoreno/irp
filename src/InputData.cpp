#include "InputData.h"

/*Modificação 18/11 por Gisele*/
InputData::InputData(){
    //ctor
}

InputData::InputData( std::vector<Driver*> drivers, std::vector<Trailer*> trailers,
        std::vector<Location*> location, std::vector<Base*> bases, std::vector<Customer*> customers,
        double** distance, int** time, int** windowSize):
        drivers_(drivers), trailers_(trailers), location_(location), bases_(bases), custumers_(customers),
        distance_(distance), time_(time), windowSize_(windowSize) {
    //ctor
}

InputData::~InputData(){
    drivers_.clear;
    trailers_.clear;
    location_.clear;
    bases_.clear;
    customers_.clear;
    //desalocar as matrizes!!!
    //dtor
}

std::vector<Driver*>* ImputData::getDrivers(){
    return &drivers_;
}
std::vector<Trailer*>* ImputData::getTrailers(){
    return &trailers;
}
std::vector<Location*>* ImputData::getLocation(){
    return &location_;
}
std::vector<Base*>* ImputData::getBases(){
    return &bases_;
}
std::vector<Customer*>* ImputData::getCustomers(){
    return &customers_;
}

