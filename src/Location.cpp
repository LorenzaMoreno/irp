#include "Location.h"

Location::Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}

Location::Location(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, Type type):
                        index_(index), neighborsBases_(neighborsBases), neighborsCustomers_(neighborsCustomers), neighborsSources_(neighborsSources), type_(type){
}

Location::~Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}

std::string Location::getIndexStr() const{
    char idx[10];
    sprintf(idx,"%c%02d",
            (type_==BASE?'B':(type_==SOURCE?'S':'C')),
            index_);
    return idx;
}
