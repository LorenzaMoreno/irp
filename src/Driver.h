#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include <vector>
class Trailer;
class Driver{
    public:
        class TimeWindow{
            private:
                int begin_;
                int end_;
            public:
                void setBegin(int begin){
                    begin_ = begin;
                }

                void setEnd(int end){
                    end_ = end;
                }

                int getBegin(){
                    return begin_;
                }

                int getEnd(){
                    return end_;
                }
        };
        Driver();
        Driver(int index, int maxDriving, std::vector<TimeWindow*> timeWindows,
std::vector<Trailer*>trailers, int minInterShift, double timeCost);
        virtual ~Driver();
        int getIndex();
        void setIndex(int index);
        int getMaxDriving();
        int setMaxDriving(int maxDriving);
        std::vector<TimeWindow*>* getTimeWindow();
        std::vector<Trailer*>* getTrailers();
        int getMinInterShift();
        void setMinInterShift(int minInterShift);
        double getTimeCost();
        void setTimeCost(double timeCost);

    private:

        int index_;
        int maxDriving_;
        std::vector<TimeWindow*> timeWindows_;
        std::vector<Trailer*> trailers_;
        int minInterShift_;
        double timeCost_;

};




#endif // DRIVER_H_INCLUDED
