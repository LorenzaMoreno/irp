#ifndef TIMEWINDOW_H
#define TIMEWINDOW_H


class TimeWindow
{
    private:
        int begin_;
        int end_;
    public:
        TimeWindow();
        TimeWindow(int, int);
        ~TimeWindow();

        int getBeginInMin() const;
        int getEndInMin() const;

        /* begin in hours */
        double getBegin() const;

        /* end in hours */
        double getEnd() const;

        void setBegin(int);
        void setEnd(int);

};

#endif // TIMEWINDOW_H
