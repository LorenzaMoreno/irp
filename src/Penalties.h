#ifndef PENALTIES_H
#define PENALTIES_H

#include <vector>
#include <string>

enum Penalty {

    DRIVER_INTERSHIFT_DURATION, // tempo em que o motorista fica parado
    DRIVER_MAX_DRIVING_TIME,
    DRIVER_TIME_WINDOWS,
    DRIVER_COST_MULTIPLIER,

    TRAILER_SHIFTS_OVERLAP, //o mesmo caminhão sendo usado ao mesmo tempo
    TRAILER_DRIVER_COMPATIBILITY,
    TRAILER_MAX_CAPACITY,
    TRAILER_NON_NEGATIVE_QUANTITY,
    TRAILER_INITIAL_QUANTITY, //quanto de carga tem naquele trailer, isso vem no arquivo de entrada
    TRAILER_COST_MULTIPLIER,

    SOURCE_MAX_TANK_CAPACITY,
    SOURCE_NEGATIVE_CAPACITY,

    CUSTOMER_MAX_TANK_CAPACITY,
    CUSTOMER_SAFETY_LEVEL,
    CUSTOMER_RUN_OUT,
    CUSTOMER_TRAILER_COMPATIBILITY,

    STOP_ARRIVAL_TIME,
    STOP_LOAD_LEVEL_CHANGED,

    NO_PENALTIES,

    NUMBER_OF
};

class Penalties
{
    public:
        Penalties();
        virtual ~Penalties();
        static void reset(int option);
        static void setValue(Penalty penalty,double value);
        static double getValue(Penalty penalty);
        static std::string toString(Penalty penalty);
    protected:
    private:
        static std::vector<double> value_;
};

#endif // PENALTIES_H
