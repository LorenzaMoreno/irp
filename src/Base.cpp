#include "Base.h"
#include "Trailer.h"

Base::Base() : Location() {
    setType(Location::BASE);
}

Base::~Base(){

}

std::string Base::toString() const {
    char data[1024];

    std::string str;

    sprintf(data,"Base %s) ",getIndexStr().c_str());

    sprintf(data,"%s Trailers=[",data);
    bool first=true;
    for(Trailer* t: trailers_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),t->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);


    sprintf(data,"%s  Drivers=[",data);
    first=true;
    for(Driver* d: drivers_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),d->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);

    str = data;
    return str;
}
