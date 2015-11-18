#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include "TimeWindow.h"
#include <vector>
class Trailer;
class Driver{
    public:
        Driver();
        Driver(int index, int maxDriving, std::vector<TimeWindow*> timeWindows, int minInterShift, double timeCost);
        Driver(int index, int maxDriving, std::vector<TimeWindow*> timeWindows, std::vector<Trailer*>trailers, int minInterShift, double timeCost);
        virtual ~Driver();

        int getIndex() const;
        int getMaxDriving() const;
        std::vector<TimeWindow*> getTimeWindow() const;
        std::vector<Trailer*> getTrailers() const;
        int getMinInterShift() const;
        double getTimeCost() const;

        void setIndex(int);
        void setMaxDriving(int);
        void setTimeWindow(std::vector<TimeWindow*>);
        void setTrailers(std::vector<Trailer*>);
        void setMinInterShift(int);
        void setTimeCost(double);

    private:

        int index_;
        int maxDriving_;
        std::vector<TimeWindow*> timeWindows_;
        std::vector<Trailer*> trailers_;
        int minInterShift_;
        double timeCost_;

};




#endif // DRIVER_H_INCLUDED
