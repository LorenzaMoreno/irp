#include "TimeWindow.h"

TimeWindow::TimeWindow()
{
    //ctor
}

TimeWindow::TimeWindow(int begin, int end): begin_(begin), end_(end)
{
    //ctor
}

TimeWindow::~TimeWindow()
{
    //dtor
}

void TimeWindow::setBegin(int begin){
    begin_ = begin;
}

void TimeWindow::setEnd(int end){
    end_ = end;
}

int TimeWindow::getBeginInMin() const{
    return begin_;
}

int TimeWindow::getEndInMin() const{
    return end_;
}

double TimeWindow::getBegin() const{
    return begin_/60.0;
}

double TimeWindow::getEnd() const{
    return end_/60.0;
}
