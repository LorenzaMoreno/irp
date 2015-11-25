#include "InputData.h"

#include <stdio.h>

InputData* InputData::instance = NULL;

InputData::InputData(){
    drivers_.clear();
    trailers_.clear();
    location_.clear();
    bases_.clear();
    customers_.clear();
    time_ = NULL;
    distance_ = NULL;
}

InputData::~InputData(){
    int i;
    int nLoc = (int)location_.size();
    int nDrivers = (int)drivers_.size();
    int nTrailers = (int)trailers_.size();


    //Release matrices and locations
    for(i=0;i<nLoc;i++){
        delete[] time_[i];
        delete[] distance_[i];
        delete location_[i]; //Locations pointers
    }
    delete[] time_;
    delete[] distance_;

    //Release drivers and trailers
    for(i=0;i<nDrivers;i++)
        delete drivers_[i]; //Drivers pointers
    for(i=0;i<nTrailers;i++)
        delete trailers_[i];//Trailers pointers

    //Clear vectors
    drivers_.clear();
    trailers_.clear();
    location_.clear();
    bases_.clear();
    customers_.clear();
}

InputData* InputData::getInstance(){
    if( instance == NULL )
        instance = new InputData();
    return instance;
}

std::vector<Driver*>* InputData::getDrivers(){
    return &(instance->drivers_);
}
std::vector<Trailer*>* InputData::getTrailers(){
    return &(instance->trailers_);
}
std::vector<Location*>* InputData::getLocation(){
    return &(instance->location_);
}
std::vector<Base*>* InputData::getBases(){
    return &(instance->bases_);
}
std::vector<Customer*>* InputData::getCustomers(){
    return &(instance->customers_);
}
