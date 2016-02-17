#ifndef ILS_H
#define ILS_H

#include "Shift.h"
#include <stdio.h>
#include <map>
#include "Solution.h"

class ILS
{
  public:
    ILS(Solution* s);
    virtual ~ILS();

    Shift* criarShift(int ixTrailer, int ixDriver, std::vector<int> loc, double tempoInicial);
    void constructor(std::vector<Customer*>* customers, int maxInstant);
    void localSearch();
  protected:
  private:
      Solution* solAtual;
};

#endif // ILS_H
