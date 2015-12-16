#ifndef BASE_H
#define BASE_H

#include "Location.h"

class Driver;
class Trailer;

class Base : public Location{
    public:
        Base();
        virtual ~Base();

        inline std::vector<Driver*>* getDrivers() {return &drivers_;}
        inline std::vector<Trailer*>* getTrailers() {return &trailers_;}

        inline void setDrivers(std::vector<Driver*> drivers){drivers_ = drivers;}
        inline void setTrailers(std::vector<Trailer*> trailers){trailers_ = trailers;}

    protected:
    private:
        std::vector<Driver*> drivers_;
        std::vector<Trailer*> trailers_;
};

#endif // BASE_H
