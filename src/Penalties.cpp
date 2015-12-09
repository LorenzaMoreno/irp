#include "Penalties.h"

Penalties::Penalties()
{
    value_.clear();
    value_.resize(Penalty::NUMBER_OF,0.0);
}

Penalties::~Penalties()
{
    value_.clear();
}


void Penalties::setValue(Penalty penalty,double value)
{
    value_[penalty] = value;
}

double Penalties::getValue(Penalty penalty)
{
    return value_[penalty];
}
