#include "Customer.h"

Customer::Customer()
{
    setType(Location::CUSTOMER);
}

Customer::Customer(double capacity, int setupTime, std::vector<double>& forecast,
                   double initialQuantity, double safetyLevel, std::vector<double>& totalLoadSafety): capacity_(capacity),
                   setupTime_(setupTime), forecast_(forecast), initialQuantity_(initialQuantity),
                   safetyLevel_(safetyLevel), totalLoadSafety_(totalLoadSafety){}

Customer::~Customer()
{
    forecast_.clear();
    allowedTrailers_.clear();
    totalLoadSafety_.clear();
}

bool Customer::isTrailerAllowed(Trailer* trailer){
    for(Trailer* t: allowedTrailers_){
        if(t->getIndex()== trailer->getIndex()){
            return true;
        }
    }
    return false;
}

std::string Customer::toString() const {
    char data[4096];

    std::string str;

    sprintf(data,"Cust %s)  SetupTime=%.1f   Cap=%-6.0f  IniQty=%-6.0f   Safety=%-6.0f",
            getIndexStr().c_str(),getSetupTime(),capacity_,initialQuantity_,safetyLevel_);

    sprintf(data,"%s Trailers=[",data);
    bool first=true;
    for(Trailer* t: allowedTrailers_){
        sprintf(data,"%s%s%d",data,(first?"":"-"),t->getIndex());
        first = false;
    }
    sprintf(data,"%s]",data);


    int tam = (int)forecast_.size();
    int n = 0, day = 0;
    double dailyAmount;
    sprintf(data,"%s\n         ",data);
    sprintf(data,"%s  Forecast=[",data);
    for(double d: forecast_){
        dailyAmount += d;
        if( n % 24 == 23 ) {
            sprintf(data,"%s%6.0f%s",data,dailyAmount,(n==tam-1?"":";"));
            if( (n/24)%10 == 9 && n < tam-1 )
                sprintf(data,"%s\n                     ",data);
            dailyAmount = 0.0;
        }
        n++;
    }
    sprintf(data,"%s]",data);

    str = data;
    return str;
}
