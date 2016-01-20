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

        inline int getIndex() const{return index_;}
        inline int getMaxDriving() const{return maxDriving_;}
        inline std::vector<TimeWindow*>* getTimeWindow(){return &timeWindows_;}
        inline std::vector<Trailer*>* getTrailers(){return &trailers_;}
        inline int getMinInterShift() const{return minInterShift_;}
        double getTimeCost() const{return timeCost_;}
        bool canDrive(Trailer* trailer);

        inline void setIndex(int index){index_ = index;}
        inline void setMaxDriving(int maxDriving){maxDriving_ = maxDriving;}
        inline void setMinInterShift(int minInterShift){minInterShift_ = minInterShift;}
        inline void setTimeCost(double timeCost){timeCost_ = timeCost;}

    private:

        int index_;
        int maxDriving_;
        std::vector<TimeWindow*> timeWindows_;
        std::vector<Trailer*> trailers_;
        int minInterShift_;
        double timeCost_;

};




#endif // DRIVER_H_INCLUDED
