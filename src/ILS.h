#ifndef ILS_H
#define ILS_H

#include "Shift.h"

class ILS
{
  public:
    ILS();
    virtual ~ILS();

    Shift* criarShift(int dia, int ixTrailer, int ixDriver);
    void constructor();
    void localSearch();
  protected:
  private:
};

#endif // ILS_H
