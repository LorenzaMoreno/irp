#include "Trailer.h"

Trailer::Trailer()
{
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost){
    //ctor
}

Trailer::Trailer(int index, double capacity, double initialQuantity, double distanceCost, std::vector<Driver*> &drivers, std::vector<Location*> &locations):
                    index_(index), capacity_(capacity), initialQuantity_(initialQuantity), distanceCost_(distanceCost), drivers_(drivers), locations_(locations){
    //ctor
}

Trailer::~Trailer()
{
    locations_.clear();
    drivers_.clear();
}
