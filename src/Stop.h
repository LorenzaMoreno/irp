#ifndef STOP_H
#define STOP_H

#include "Location.h"

class Stop
{
    public:
        Stop();
        Stop(double quantity, int arriveTime, Location* location);
        virtual ~Stop();

        Location* getLocation() const;
        int getArriveTime() const;
        double getQuantity() const;

        void setLocation(Location*);
        void setArriveTime(int);
        void setQuantity(double);

    private:
        Location* location_;
        int arriveTime_;
        double quantity_;
};

#endif // STOP_H
