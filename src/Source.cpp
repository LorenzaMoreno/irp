#include "Source.h"

Source::Source()
{
    setType(Location::SOURCE);
}

Source::Source(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, int setupTime, Type type)
                :Location(index, neighborsBases, neighborsCustomers, neighborsSources, type), setupTime_(setupTime)
{
    //ctor
}

Source::~Source()
{
    //dtor
}

std::string Source::toString() const {
    char data[1024];

    std::string str;

    sprintf(data,"Srce %s)  SetupTime=%.1f",getIndexStr().c_str(),getSetupTime());


    str = data;
    return str;
}
