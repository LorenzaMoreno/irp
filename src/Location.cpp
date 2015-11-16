#include "Location.h"

Location::Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}

Location::Location(int index, std::vector<Base*> neighborsBases,
std::vector<Customer*> neighborsCustomers,
std::vector<Source*> neighborsSources):
index_(index),
neighborsBases_(neighborsBases),
neighborsCustomers_(neighborsCustomers),
neighborsSources_(neighborsSources){
    //ctor
}

Location::~Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}

int Location::getIndex(){
    return index_;
}

std::vector<Base*>* Location::getNeighborsBases(){
    return &neighborsBases_;
}

std::vector<Customer*>* Location::getNeighborsCustomers(){
    return &neighborsCustomers_;
}

std::vector<Source*>* Location::getNeighborsSources(){
    return &neighborsSources_;
}
