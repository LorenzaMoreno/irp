#ifndef LOCATION_H
#define LOCATION_H

#include <vector>
class Base;
class Customer;
class Source;

class Location
{
    public:
        Location();
        Location(int index, std::vector<Base*> neighborsBases,
                    std::vector<Customer*> neighborsCustomers,
                    std::vector<Source*> neighborsSources);
        enum Type{BASE, CUSTOMER, SOURCE};
        virtual ~Location();
        int getIndex();
        std::vector<Base*>* getNeighborsBases();
        std::vector<Customer*>* getNeighborsCustomers();
        std::vector<Source*>* getNeighborsSources();

    protected:
        int index_;
        std::vector<Base*> neighborsBases_;
        std::vector<Customer*> neighborsCustomers_;
        std::vector<Source*> neighborsSources_;
        Type type_;
};

#endif // LOCATION_H
