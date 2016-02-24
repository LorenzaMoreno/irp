#include "InputData.h"
#include "Penalties.h"

#include <stdio.h>
#include <algorithm> // sort


InputData* InputData::instance = NULL;

InputData::InputData(){
    drivers_.clear();
    trailers_.clear();
    locations_.clear();
    bases_.clear();
    sources_.clear();
    customers_.clear();
    time_ = NULL;
    timeInMinutes_ = NULL;
    distance_ = NULL;
    Penalties::reset(1);
}

InputData::~InputData(){
    int i;
    int nLoc = (int)locations_.size();
    int nDrivers = (int)drivers_.size();
    int nTrailers = (int)trailers_.size();


    //Release matrices and locations
    for(i=0;i<nLoc;i++){
        delete[] time_[i];
        delete[] timeInMinutes_[i];
        delete[] distance_[i];
        delete locations_[i]; //Locations pointers
    }
    delete[] time_;
    delete[] timeInMinutes_;
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
    Driver* driver = instance->drivers_.at(id);
    if( driver->getIndex() == id )
        return driver;
    else
    for(Driver* d : drivers_){
        if(d->getIndex() == id){
            return d;
        }
    }
    return NULL;
}

Trailer* InputData::findTrailer(int id){
    Trailer* t = instance->trailers_.at(id);
    if( t->getIndex() == id )
        return t;
    else
    for(Trailer* t2 : trailers_)
        if(t2->getIndex() == id)
            return t2;
    return NULL;
}

Location* InputData::findLocation(int id){
    Location* loc = instance->locations_.at(id);
    if( loc->getIndex() == id )
        return loc;
    else
    for(Location* l : locations_)
        if(l->getIndex() == id)
            return l;
    return NULL;
}

double InputData::getDistance(int origin, int destination){
    return instance->distance_[origin][destination];
}

double InputData::getTime(int origin, int destination){
    return instance->time_[origin][destination];
}

int InputData::getTimeInMinutes(int origin, int destination){
    return instance->timeInMinutes_[origin][destination];
}

void InputData::resizeTimeAndDistanceMatrices(int numLocations){
    int i;
    if( distance_ == NULL ){
        distance_ = new double*[numLocations];
        for(i=0;i<numLocations;i++)
            distance_[i] = new double[numLocations];
    }
    if( time_ == NULL ){
        time_ = new double*[numLocations];
        for(i=0;i<numLocations;i++)
            time_[i] = new double[numLocations];
    }
    if( timeInMinutes_ == NULL ){
        timeInMinutes_ = new int*[numLocations];
        for(i=0;i<numLocations;i++)
            timeInMinutes_[i] = new int[numLocations];
    }
}

struct Comparator {
    Comparator(Location* location) { this->location = location; }
    bool operator () (Location* l1, Location* l2) {
        InputData* input = InputData::getInstance();
        double* locationDistances = input->getDistancesFrom(location->getIndex());

        double dist1 = locationDistances[l1->getIndex()];
        double dist2 = locationDistances[l2->getIndex()];

        return (dist1 < dist2);
    }
    Location* location;
};

void InputData::calcNeighborsLocations(){
    for(Location* location : *(InputData::getLocations())){
        for(Location* loc : *(InputData::getLocations())){
            if(loc->getIndex() != location->getIndex()){
                switch(loc->getType()){
                    case Location::BASE:{
                        location->getNeighborsBases()->push_back((Base*)loc);
                    }break;
                    case Location::SOURCE:{
                        location->getNeighborsSources()->push_back((Source*)loc);
                    }break;
                    case Location::CUSTOMER:{
                        location->getNeighborsCustomers()->push_back((Customer*)loc);
                    }break;
                }
            }
        }
        std::sort((location->getNeighborsBases())->begin(),(location->getNeighborsBases())->end(),Comparator(location));
        std::sort((location->getNeighborsCustomers())->begin(),(location->getNeighborsCustomers())->end(),Comparator(location));
        std::sort((location->getNeighborsSources())->begin(),(location->getNeighborsSources())->end(),Comparator(location));


        printf("\n Lista de bases vizinhos do location de index %d\n",location->getIndex());
        for(Base* b : *(location->getNeighborsBases())){
            printf("-> %d",b->getIndex());
        }

        printf("\n Lista de customers vizinhos do location de index %d\n",location->getIndex());
        for(Customer* b : *(location->getNeighborsCustomers())){
            printf("-> %d",b->getIndex());
        }

        printf("\n Lista de sources vizinhos do location de index %d\n",location->getIndex());
        for(Source* b : *(location->getNeighborsSources())){
            printf("-> %d",b->getIndex());
        }

//        printf("\n Lista de bases vizinhos do location de index %d\n",location->getIndex());
//        for(Base* b : *(location->getNeighborsBases())){
//            printf("-> %d",b->getIndex());
//        }
//        printf("\n Lista de customers vizinhos do location de index %d\n",location->getIndex());
//        for(Customer* b : *(location->getNeighborsCustomers())){
//            printf("-> %d",b->getIndex());
//        }
//        printf("\n Lista de sources vizinhos do location de index %d\n",location->getIndex());
//        printf("\n Lista de sources vizinhos do location de index %d\n",location->getIndex());
//        for(Source* b : *(location->getNeighborsSources())){
//            printf("-> %d",b->getIndex());
//        }

    }
}


