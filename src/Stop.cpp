#include <stdio.h>
#include "Stop.h"

Stop::Stop(): location_(NULL)
{
    shift_ = NULL;
    quantity_ = 0;
    arriveTime_ = 0;
}

Stop::Stop(double quantity, double arriveTime, Location* location): location_(location), arriveTime_(arriveTime), quantity_(quantity)
{

};

Stop::~Stop()
{
    //dtor
}

std::string Stop::toString() const {
    std::string str;
    char data[100],time[50];

    sprintf(time,"%.1f",arriveTime_);

    if( location_ != NULL ){
        sprintf(data,"%s    ",location_->getIndexStr().c_str());
        sprintf(time,"%s-%.1f",time,arriveTime_+location_->getSetupTime());
    }
    else {
        sprintf(data,"XXX    ");
        sprintf(time,"%s-   ",time);
    }

    sprintf(data,"%sQty=%-8.0f",data,quantity_);

    sprintf(data,"%sTime=%s",data,time);

    if(shift_ == NULL)
        sprintf(data,"%s [NO SHIFT]",data);

    str = data;
    return str;
}
