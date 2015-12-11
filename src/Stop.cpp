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
