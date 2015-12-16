#include "InputData.h"

#include <stdio.h>

InputData* InputData::instance = NULL;

InputData::InputData(){
    drivers_.clear();
    trailers_.clear();
    locations_.clear();
    bases_.clear();
    sources_.clear();
    customers_.clear();
    time_ = NULL;
    distance_ = NULL;
}

InputData::~InputData(){
    int i;
    int nLoc = (int)locations_.size();
    int nDrivers = (int)drivers_.size();
    int nTrailers = (int)trailers_.size();


    //Release matrices and locations
    for(i=0;i<nLoc;i++){
        delete[] time_[i];
        delete[] distance_[i];
        delete locations_[i]; //Locations pointers
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
    locations_.clear();
    bases_.clear();
    sources_.clear();
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
std::vector<Location*>* InputData::getLocations(){
    return &(instance->locations_);
}
std::vector<Base*>* InputData::getBases(){
    return &(instance->bases_);
}
std::vector<Source*>* InputData::getSources(){
    return &(instance->sources_);
}
std::vector<Customer*>* InputData::getCustomers(){
    return &(instance->customers_);
}

Driver* InputData::findDriver(int id){
    //if it is sequencial
    return instance->drivers_.at(id - 1);
    //else
//    for(Driver* d : drivers_){
//        if(d->getIndex() == id){
//            return d;
//        }
//    }
}

Trailer* InputData::findTrailer(int id){
    //if it is sequencial
    return instance->trailers_.at(id - 1);
    //else
//    for(Trailer* t : trailers_){
//        if(t->getIndex() == id){
//            return t;
//        }
//    }
}

Location* InputData::findLocation(int id){
    //if it is sequencial
    return instance->locations_.at(id - 1);
    //else
//    for(Location* l : locations_){
//        if(l->getIndex() == id){
//            return l;
//        }
//    }
}

double InputData::getDistance(int origin, int destination){
    return instance->distance_[origin][destination];
}

int InputData::getTime(int origin, int destination){
    return instance->time_[origin][destination];
}
