#ifndef INPUTDATA_H
#define INPUTDATA_H

#include "Driver.h"
#include "Trailer.h"
#include "Location.h"
#include "Base.h"
#include "Source.h"
#include "Customer.h"

class InputData
{
    public:
        InputData();

        static InputData* getInstance();

        static std::vector<Driver*>* getDrivers();
        static std::vector<Trailer*>* getTrailers();
        static std::vector<Location*>* getLocations();
        static std::vector<Base*>* getBases();
        static std::vector<Source*>* getSources();
        static std::vector<Customer*>*  getCustomers();

        Driver* findDriver(int id);
        Trailer* findTrailer(int id);
        Location* findLocation(int id);

        double getDistance(int origin, int destination);
        int getTime(int origin, int destination);

        virtual ~InputData();
    protected:
    private:
        std::vector<Driver*> drivers_;
        std::vector<Trailer*> trailers_;
        std::vector<Location*> locations_;
        std::vector<Base*> bases_;
        std::vector<Source*> sources_;
        std::vector<Customer*> customers_;
        double** distance_;
        int** time_;
        //int** windowSize_;

        //find de cada um *
        //acesso e criação de matriz

        static InputData* instance;
};
#endif // INPUTDATA_H
