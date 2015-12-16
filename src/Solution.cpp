#include "Solution.h"
#include "Location.h"
#include <cmath>
#include <cstdlib>

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
    customer_.clear();
}

void Solution::clear(){
    for(std::vector<Shift*> a : driverInst_){
        a.clear();
    }
    driverInst_.clear();

    for(std::vector<Shift*> a : trailerInst_){
        a.clear();
    }
    trailerInst_.clear();

    for(std::vector<double> a : stockLevelInst_){
        a.clear();
    }
    stockLevelInst_.clear();

    customer_.clear();

    infeasibilityCost_=-1;
    cost_=-1;
}

void Solution::reset(){
    clear();
    driverInst_.resize(InputData::getDrivers()->size());
    for(std::vector<Shift*> a : driverInst_){
        a.resize(InputData::getNumInst(),NULL);
    }

    trailerInst_.resize(InputData::getTrailers()->size());
    for(std::vector<Shift*> a : trailerInst_){
        a.resize(InputData::getNumInst(),NULL);
    }

    stockLevelInst_.resize(InputData::getLocation()->size());
    int i=0;
    for(Location* loc : *(InputData::getLocation())){
        switch(loc->getType()){
            case Location::BASE:{
                stockLevelInst_[i].clear();
            }break;
            case Location::SOURCE:{
                stockLevelInst_[i].resize(InputData::getNumInst(),INFINITY);
            }break;
            case Location::CUSTOMER:{
                Customer* c=(Customer*)loc;
                stockLevelInst_[i].resize(InputData::getNumInst(),c->getInitialQualtity());
                for(unsigned int j=1;j< c->getForecast()->size();j++){
                    stockLevelInst_[i][j]=
                        stockLevelInst_[i][j-1]-
                        (*(c->getForecast()))[j];
                }
            }break;
        }
        i++;
    }



    customer_.clear();
}

void Solution::insertShift(Shift* shift){

}

void Solution::removeShift(Shift* shift){
}

void Solution::insertStopInShift(Shift* shift, Stop* stop){
}

void Solution::removeStopFromShift(Shift* shift, Stop* stop){
}
