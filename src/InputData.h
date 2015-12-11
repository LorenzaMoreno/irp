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

        static InputData* getInstance();

        static std::vector<Driver*>* getDrivers();
        static std::vector<Trailer*>* getTrailers();
        static std::vector<Location*>* getLocation();
        static std::vector<Base*>* getBases();
        static std::vector<Customer*>*  getCustomers();

        Driver* findDriver(int id);
        Trailer* findTrailer(int id);
        Location* findLocation(int id);

        double getDistance(int origin, int destination);
        int getTime(int origin, int destination);

        inline static int getHorizon(){return instance->horizon;}
        inline static int getUnit(){return instance->unit;}
        inline static int getNumInst(){return instance->numInst;}


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
        int horizon, unit, numInst;
        //int** windowSize_;

        //find de cada um *
        //acesso e criação de matriz

        static InputData* instance;
};
#endif // INPUTDATA_H
