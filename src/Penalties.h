#ifndef PENALTIES_H
#define PENALTIES_H

#include <vector>

enum Penalty {

    DRIVER_INTERSHIFT_DURATION,
    DRIVER_MAX_DRIVING_TIME,
    DRIVER_TIME_WINDOWS,
    DRIVER_COST_MULTIPLIER,

    TRAILER_SHIFTS_OVERLAP,
    TRAILER_DRIVER_COMPATIBILITY,
    TRAILER_MAX_CAPACITY,
    TRAILER_NON_NEGATIVE_QUANTITY,
    TRAILER_INITIAL_QUANTITY,
    TRAILER_COST_MULTIPLIER,

    SOURCE_MAX_TANK_CAPACITY,
    SOURCE_NON_NEGATIVE_CAPACITY,

    CUSTOMER_MAX_TANK_CAPACITY,
    CUSTOMER_SAFETY_LEVEL,
    CUSTOMER_RUN_OUT,
    CUSTOMER_TRAILER_COMPATIBILITY,

    STOP_ARRIVAL_TIME,
    STOP_LOAD_OPERATION_TIME,
    STOP_LOAD_LEVEL_CHANGED,

    NUMBER_OF
};

class Penalties
{
    public:
        Penalties();
        virtual ~Penalties();
        static void setValue(Penalty penalty,double value);
        static double getValue(Penalty penalty);
    protected:
    private:
        std::vector<double> value_;
};

#endif // PENALTIES_H
