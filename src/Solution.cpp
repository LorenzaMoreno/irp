#include "Solution.h"
#include "Location.h"
#include <cmath>
#include <cstdlib>
#include <utility>

Solution::Solution()
{
    //ctor
}

Solution::Solution(double cost, double infeasibilityCost):
                cost_(cost), infeasibilityCost_(infeasibilityCost){
}

Solution::~Solution()
{
    driverInst_.clear();
    trailerInst_.clear();
    stockLevelInst_.clear();
    safetyLevelInst_.clear();
}

void Solution::clear(){
    for(std::vector<std::vector<Shift*> > c : driverInst_){
        for(std::vector<Shift*> b : c){
            b.clear();
        }
        c.clear();
    }
    driverInst_.clear();

    for(std::vector<std::vector<Shift*> > c : trailerInst_){
        for(std::vector<Shift*> b : c){
            b.clear();
        }
        c.clear();
    }
    trailerInst_.clear();

    for(std::vector<double> a : stockLevelInst_){
        a.clear();
    }
    stockLevelInst_.clear();

    for(std::vector<std::vector<Stop*> > a : locationInstStop_){
        for(std::vector<Stop*> b : a)
            b.clear();
        a.clear();
    }
    locationInstStop_.clear();

    safetyLevelInst_.clear();

    infeasibilityCost_=-1;
    cost_=-1;
}

void Solution::reset(){
    std::vector<Shift*> v;
    v.resize(2,NULL);
    v.clear();

    clear();
    driverInst_.resize(InputData::getDrivers()->size());
    for(std::vector<std::vector<Shift*> > a : driverInst_){
        a.resize(InputData::getNumInst(),v);
    }

    trailerInst_.resize(InputData::getTrailers()->size());
    for(std::vector<std::vector<Shift*> > a : trailerInst_){
        a.resize(InputData::getNumInst(),v);
    }

    safetyLevelInst_.clear();
    stockLevelInst_.resize(InputData::getLocations()->size());
    int i=0;
    bool safetyLevelReached = false;
    for(Location* loc : *(InputData::getLocations())){
        switch(loc->getType()){
            case Location::BASE:{
                stockLevelInst_[i].clear();
            }break;
            case Location::SOURCE:{
                stockLevelInst_[i].resize(InputData::getNumInst(),INFINITY);
            }break;
            case Location::CUSTOMER:{
                Customer* c=(Customer*)loc;
                stockLevelInst_[i].resize(InputData::getNumInst(),c->getInitialQuantity());
                for(unsigned int j=1;j< c->getForecast()->size();j++){
                    stockLevelInst_[i][j]=stockLevelInst_[i][j-1]-(*(c->getForecast()))[j];
                    if(!safetyLevelReached && stockLevelInst_[i][j]<c->getSafetyLevel()){
                        safetyLevelReached = true;
                        safetyLevelInst_.insert(std::make_pair(j,c));
                    }
                }
            }break;
        }
        i++;
    }

    std::vector<Stop*> s;
    s.resize(2,NULL);
    s.clear();
    locationInstStop_.resize(InputData::getLocations()->size());
    for(std::vector<std::vector<Stop*> > a : locationInstStop_){
        a.resize(InputData::getNumInst(),s);
    }
}


bool Solution::checkShift(Shift* shift){
    /*
        1º
    */
}

bool Solution::checkStop(Stop* stop){
    /*
        1º Does the Shift Fits?
            Verify if there is not another Stop at the instant
        2º Conflict with another driver/trailer
            Verify if there is another Stop in the same location at the same instant
    */
}

void Solution::insertShift(Shift* shift){

}

void Solution::removeShift(Shift* shift){
}

void Solution::insertStopInShift(Shift* shift, Stop* stop){
}

void Solution::removeStopFromShift(Shift* shift, Stop* stop){
}
