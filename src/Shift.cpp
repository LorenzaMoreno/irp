#include "Shift.h"

Shift::Shift(){
    //ctor
}

Shift::Shift(double cost, Driver* driver, Trailer* trailer) : cost_(cost), driver_(driver), trailer_(trailer){
    //ctor
}


Shift::~Shift(){
    stops_.clear();
}
