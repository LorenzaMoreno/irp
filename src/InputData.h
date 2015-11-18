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
        InputData(std::vector<Driver*> &drivers, std::vector<Trailer*> &trailers, std::vector<Location*> &location, std::vector<Base*> &bases, std::vector<Customer*> &customers, double** distance, int** time, int** windowSize);

        std::vector<Driver*> getDrivers() const;
        std::vector<Trailer*> getTrailers() const;
        std::vector<Location*> getLocation() const;
        std::vector<Base*> getBases() const;
        std::vector<Customer*>  getCustomers() const;

        void setDrivers(std::vector<Driver*>);
        void setTrailers(std::vector<Trailer*>);
        void setLocation(std::vector<Location*>);
        void setBases(std::vector<Base*>);
        void setCustomers( std::vector<Customer*>);

        virtual ~InputData();
    protected:
    private:
        std::vector<Driver*> drivers_;
        std::vector<Trailer*> trailers_;
        std::vector<Location*> location_;
        std::vector<Base*> bases_;
        std::vector<Customer*> customers_;
        double** distance_;
        int** time_;
        int** windowSize_;
};
#endif // INPUTDATA_H
