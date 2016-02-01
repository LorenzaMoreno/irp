#include "Penalties.h"
std::vector<double> Penalties::value_;

Penalties::Penalties()
{
    value_.clear();
    value_.resize(Penalty::NUMBER_OF,0.0);
}

Penalties::~Penalties()
{
    value_.clear();
}

void Penalties::reset(int option)
{
    value_.clear();
    value_.resize(Penalty::NUMBER_OF,1.0);
}


void Penalties::setValue(Penalty penalty,double value)
{
    value_[penalty] = value;
}

double Penalties::getValue(Penalty penalty)
{
    return value_[penalty];
}

std::string Penalties::toString(Penalty penalty){
    switch(penalty){
    case DRIVER_INTERSHIFT_DURATION:
        return "DRIVER_INTERSHIFT_DURATION";

    case DRIVER_MAX_DRIVING_TIME:
        return "DRIVER_MAX_DRIVING_TIME";

    case DRIVER_TIME_WINDOWS:
        return "DRIVER_TIME_WINDOWS";

    case DRIVER_COST_MULTIPLIER:
        return "DRIVER_COST_MULTIPLIER";

    case TRAILER_SHIFTS_OVERLAP:
        return "TRAILER_SHIFTS_OVERLAP";

    case TRAILER_DRIVER_COMPATIBILITY:
        return "TRAILER_DRIVER_COMPATIBILITY";

    case TRAILER_MAX_CAPACITY:
        return "TRAILER_MAX_CAPACITY";

    case TRAILER_NON_NEGATIVE_QUANTITY:
        return "TRAILER_NON_NEGATIVE_QUANTITY";

    case TRAILER_INITIAL_QUANTITY:
        return "TRAILER_INITIAL_QUANTITY";

    case TRAILER_COST_MULTIPLIER:
        return "TRAILER_COST_MULTIPLIER";

    case SOURCE_MAX_TANK_CAPACITY:
        return "SOURCE_MAX_TANK_CAPACITY";

    case SOURCE_NEGATIVE_CAPACITY:
        return "SOURCE_NEGATIVE_CAPACITY";

    case CUSTOMER_MAX_TANK_CAPACITY:
        return "CUSTOMER_MAX_TANK_CAPACITY";

    case CUSTOMER_SAFETY_LEVEL:
        return "CUSTOMER_SAFETY_LEVEL";

    case CUSTOMER_RUN_OUT:
        return "CUSTOMER_RUN_OUT";

    case CUSTOMER_TRAILER_COMPATIBILITY:
        return "CUSTOMER_TRAILER_COMPATIBILITY";

    case STOP_ARRIVAL_TIME:
        return "STOP_ARRIVAL_TIME";

    case STOP_LOAD_LEVEL_CHANGED:
        return "STOP_LOAD_LEVEL_CHANGED";
    }
}
