#ifndef ILS_H
#define ILS_H

#include "Shift.h"

class ILS
{
  public:
    ILS();
    virtual ~ILS();

    Shift* criarShift(int ixTrailer, int ixDriver, std::vector<int> loc, Solution* solAtual);
    void constructor();
    void localSearch();
  protected:
  private:
};

#endif // ILS_H
