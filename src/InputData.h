#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "Driver.h"
#include "Trailer.h"
#include "Location.h"
#include "Base.h"
#include "Customer.h"

class InputData
{
    public:
        InputData();
        virtual ~InputData();
    protected:
    private:
        std::vector<Driver*>* drivers_;
        std::vector<Trailer*>* trailers_;
        std::vector<Location*>* location_;
        std::vector<Base*>* bases_;
        std::vector<Customer*>* customers_;
        double** distance_;
        int** time_;
        int** windowSize_;
};

#endif // INPUTDATA_H
