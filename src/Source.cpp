#include "Source.h"

Source::Source()
{
    //ctor
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
