#ifndef STOP_H
#define STOP_H

#include "Location.h"

#include <string>
#include <sstream>

class Shift;

class Stop{
    public:
        Stop();
        Stop(double quantity, int arriveTime, Location* location);
        virtual ~Stop();

        operator std::string(){
            std::stringstream stream;
            stream << "Location: " << location_->getIndex() << std::endl;
            stream << "Quantity: " << quantity_ << std::endl;
            stream << "Arrive Time: " << arriveTime_ << std::endl;
            return stream.str();
        }

        inline Location* getLocation() const{return location_;}
        inline int getArriveTime() const{return arriveTime_;}
        inline double getQuantity() const{return quantity_;}
        inline Shift* getShifts() {return shift_;}

        inline void setLocation(Location* location){location_ = location;}
        inline void setArriveTime(int arriveTime){arriveTime_ = arriveTime;}
        inline void setQuantity(double quantity){quantity_ = quantity;}
        inline void setShift(Shift* shift){shift_ = shift;}

    private:
        Location* location_;
        double arriveTime_;
        double quantity_;
        Shift* shift_;
};

#endif // STOP_H
