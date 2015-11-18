#include "Driver.h"


Driver::Driver(){
    timeWindows_.clear();
    trailers_.clear();
}

Driver::Driver(int index, int maxDriving, std::vector<TimeWindow*> timeWindows, int minInterShift, double timeCost):
                index_(index), maxDriving_(maxDriving), timeWindows_(timeWindows), minInterShift_(minInterShift), timeCost_(timeCost){

}

Driver::Driver(int index, int maxDriving, std::vector<TimeWindow*> timeWindows, std::vector<Trailer*>trailers, int minInterShift, double timeCost):
                index_(index), maxDriving_(maxDriving), timeWindows_(timeWindows), trailers_(trailers), minInterShift_(minInterShift), timeCost_(timeCost){

}

Driver::~Driver(){
    timeWindows_.clear();
    trailers_.clear();

}

int Driver::getIndex() const{
    return index_;
}

void Driver::setIndex(int index){
    index_ = index;
}

int Driver::getMaxDriving() const{
    return maxDriving_;
}

void Driver::setMaxDriving(int maxDriving){
    maxDriving_ = maxDriving;
}

std::vector<TimeWindow*> Driver::getTimeWindow() const{
    return timeWindows_;
}

void Driver::setTimeWindow(std::vector<TimeWindow*> timeWindow){
    timeWindows_ = timeWindow;
}

std::vector<Trailer*> Driver::getTrailers() const{
    return trailers_;
}

void Driver::setTrailers(std::vector<Trailer*> trailers){
    trailers_ = trailers;
}

int Driver::getMinInterShift() const{
    return minInterShift_;
}

void Driver::setMinInterShift(int minInterShift){
    minInterShift_= minInterShift;
}

double Driver::getTimeCost() const{
    return timeCost_;
}

void Driver::setTimeCost(double timeCost){
    timeCost_=timeCost;
}



















