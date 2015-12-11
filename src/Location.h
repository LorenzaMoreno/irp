#ifndef LOCATION_H
#define LOCATION_H

#include <vector>
class Base;
class Customer;
class Source;

class Location
{
    public:
        enum Type{BASE, CUSTOMER, SOURCE};
        Location();
        Location(int index, std::vector<Base*> &neighborsBases, std::vector<Customer*> &neighborsCustomers, std::vector<Source*> &neighborsSources, Type type);
        virtual ~Location();
        inline int getIndex() const{return index_;}
        inline std::vector<Base*>* getNeighborsBases(){return &neighborsBases_;}
        inline std::vector<Customer*>* getNeighborsCustomers(){return &neighborsCustomers_;}
        inline std::vector<Source*>* getNeighborsSources(){return &neighborsSources_;}
        inline Type getType() const{return type_;}

        inline void setIndex(int index){index_ = index;}
        inline void setType(Type type){type_ = type;}

    protected:
        int index_;
        std::vector<Base*> neighborsBases_;
        std::vector<Customer*> neighborsCustomers_;
        std::vector<Source*> neighborsSources_;
        Type type_;
};

#endif // LOCATION_H
