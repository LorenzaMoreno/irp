#include "Source.h"

Source::Source()
{
    //ctor
}

Source::Source(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, int setupTime)
                :Location(index, neighborsBases, neighborsCustomers, neighborsSources), setupTime_(setupTime)
{
    //ctor
}

Source::~Source()
{
    //dtor
}
