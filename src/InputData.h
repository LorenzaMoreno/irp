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
        void calcNeighborsLocations();

        Driver* findDriver(int id);
        Trailer* findTrailer(int id);
        Location* findLocation(int id);

        static double getDistance(int origin, int destination);
        static double getTime(int origin, int destination);
        static int getTimeInMinutes(int origin, int destination);

        inline static int getHorizon(){return instance->horizon_;}
        inline static int getUnit(){return instance->unit_;}
        inline static int getNumInst(){return instance->numInst_;}

        void resizeTimeAndDistanceMatrices(int numLocations);
        inline double* getDistancesFrom(int orig) {return distance_[orig];}
        inline double* getTimeFrom(int orig) {return time_[orig];}
        inline int* getTimeInMinutesFrom(int orig) {return timeInMinutes_[orig];}

        inline static void setHorizon(int horizon){instance->horizon_ = horizon;}
        inline static void setUnit(int unit){instance->unit_ = unit;}
        inline static void setNumInst(int numInst){instance->numInst_ = numInst;}

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
        double** time_;
        int** timeInMinutes_;
        int horizon_, unit_, numInst_;
        //int** windowSize_;

        //find de cada um *
        //acesso e criação de matriz

        static InputData* instance;
};
#endif // INPUTDATA_H
