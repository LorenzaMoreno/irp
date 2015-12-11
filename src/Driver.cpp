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
