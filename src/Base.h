#ifndef BASE_H
#define BASE_H

#include "Location.h"
class Base : public Location
{
    public:
        Base();
        Base(int index, std::vector<Base*> neighborsBases,
            std::vector<Customer*> neighborsCustomers,
            std::vector<Source*> neighborsSources);
        virtual ~Base();
    protected:
    private:
};

#endif // BASE_H
