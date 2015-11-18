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

int TimeWindow::getBegin() const{
    return begin_;
}

int TimeWindow::getEnd() const{
    return end_;
}
