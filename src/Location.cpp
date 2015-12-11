#include "Location.h"

Location::Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}

Location::Location(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources):
                        index_(index), neighborsBases_(neighborsBases), neighborsCustomers_(neighborsCustomers), neighborsSources_(neighborsSources){
}

Location::~Location(){
    neighborsBases_.clear();
    neighborsCustomers_.clear();
    neighborsSources_.clear();
}
