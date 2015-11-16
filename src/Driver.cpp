#include "Driver.h"


Driver::Driver(){
    timeWindows_.clear();
    trailers_.clear();
}

Driver::Driver(int index, int maxDriving,
std::vector<Driver::TimeWindow*> timeWindows,
std::vector<Trailer*>trailers,
int minInterShift,
double timeCost):index_(index),
maxDriving_(maxDriving),
timeWindows_(timeWindows),
trailers_(trailers),
minInterShift_(minInterShift),
timeCost_(timeCost){

}

Driver::~Driver(){
    timeWindows_.clear();
    trailers_.clear();

}

int Driver::getIndex(){
    return index_;
}

void Driver::setIndex(int index){
    index_ = index;
}

int Driver::getMaxDriving(){
    return maxDriving_;
}

int Driver::setMaxDriving(int maxDriving){
    maxDriving_ = maxDriving;
}

std::vector<Driver::TimeWindow*>* Driver::getTimeWindow(){
    return &timeWindows_;
}

std::vector<Trailer*>* Driver::getTrailers(){
    return &trailers_;
}

int Driver::getMinInterShift(){
    return minInterShift_;
}

void Driver::setMinInterShift(int minInterShift){
    minInterShift_= minInterShift;
}

double Driver::getTimeCost(){
    return timeCost_;
}

void Driver::setTimeCost(double timeCost){
    timeCost_=timeCost;
}



















