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

int Location::getIndex(){
    return index_;
}

std::vector<Base*> Location::getNeighborsBases() const{
    return neighborsBases_;
}

std::vector<Customer*> Location::getNeighborsCustomers() const{
    return neighborsCustomers_;
}

std::vector<Source*> Location::getNeighborsSources() const{
    return neighborsSources_;
}

void Location::setIndex(int index){
    index_ = index;
}
void Location::setNeighborsBases(std::vector<Base*> neighborsBases){
    neighborsBases_ = neighborsBases;
}
void Location::setNeighborsCustomers(std::vector<Customer*> neighborsCustomers){
    neighborsCustomers_ = neighborsCustomers;
}
void Location::setNeighborsSources(std::vector<Source*> neighborsSources){
    neighborsSources_ = neighborsSources;
}
