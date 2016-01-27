#include "Driver.h"
#include "Trailer.h"
#include <stdio.h>

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

bool Driver::canDrive(Trailer* trailer){
    for(Trailer* t: trailers_){
        if(t->getIndex()== trailer->getIndex()){
            return true;
        }
    }
    return false;
}

std::string Driver::toString() const{
    char data[2048],twstr[10];
    std::string str;

    sprintf(data,"Driver %2d)  MaxDriv=%-3d  MinInterShft=%-3d  TimeCost=%-7.2f ",
            index_,maxDriving_,minInterShift_,timeCost_);

    sprintf(data,"%s Trailers=[",data);
    bool first=true;
    for(Trailer* t: trailers_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),t->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);

    int numTW = (int)timeWindows_.size();
    int n = 0;
    sprintf(data,"%s\n         ",data);
    sprintf(data,"%s   TimeWindows=[",data);
    for(TimeWindow* tw: timeWindows_){
        sprintf(twstr,"%.0f-%.0f",tw->getBegin(),tw->getEnd());
        sprintf(data,"%s%10s%s",data,twstr,(n==numTW-1?"":";"));
        if( n%8 == 7 )
            sprintf(data,"%s\n                         ",data);
        n++;
    }
    sprintf(data,"%s]",data);

    str = data;
    return str;
}
