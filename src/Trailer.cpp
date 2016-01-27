#include "Trailer.h"

Trailer::Trailer()
{
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost){
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost, std::vector<Driver*> &drivers, std::vector<Location*> &locations):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost), drivers_(drivers), locations_(locations){
    //ctor
}

Trailer::~Trailer()
{
    locations_.clear();
    drivers_.clear();
}


std::string Trailer::toString() const{
    char data[2048];
    std::string str;

    sprintf(data,"Trailer %2d)  Cap=%-5.0f  IniQty=%-5.0f  DistCost=%-7.2f ",
            index_,capacity_,initialQuantity_,distanceCost_);

    sprintf(data,"%s Drivers=[",data);
    bool first=true;
    for(Driver* d: drivers_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),d->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);

    sprintf(data,"%s   Locations=[",data);
    first=true;
    for(Location* l: locations_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),l->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);

    str = data;
    return str;
}
