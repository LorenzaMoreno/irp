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
        Location(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources);
        enum Type{BASE, CUSTOMER, SOURCE};
        virtual ~Location();
        inline int getIndex() const{return index_;}
        inline std::vector<Base*>* getNeighborsBases(){return &neighborsBases_;}
        inline std::vector<Customer*>* getNeighborsCustomers(){return &neighborsCustomers_;}
        inline std::vector<Source*>* getNeighborsSources(){return &neighborsSources_;}

        inline void setIndex(int index){index_ = index;}

    protected:
        int index_;
        std::vector<Base*> neighborsBases_;
        std::vector<Customer*> neighborsCustomers_;
        std::vector<Source*> neighborsSources_;
        Type type_;
};

#endif // LOCATION_H
