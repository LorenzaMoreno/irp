#include <stdio.h>
#include "Stop.h"

Stop::Stop(): location_(NULL)
{
    //ctor
}

Stop::Stop(double quantity, int arriveTime, Location* location): location_(location), arriveTime_(arriveTime), quantity_(quantity)
{

};

Stop::~Stop()
{
    //dtor
}

Location* Stop::getLocation() const{
    return location_;
}

void Stop::setLocation(Location* location){
    location_ = location;
}

int Stop::getArriveTime() const{
    return arriveTime_;
}

void Stop::setArriveTime(int arrivetTime){
    arriveTime_ = arrivetTime;
}

double Stop::getQuantity() const{
    return quantity_;
}

void Stop::setQuantity(double quantity){
    quantity_ = quantity;
}
