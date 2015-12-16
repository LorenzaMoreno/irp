#include "Customer.h"

Customer::Customer()
{
    //ctor
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
