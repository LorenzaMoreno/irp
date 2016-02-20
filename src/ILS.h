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

    Customer* getCustomerMaisProximo(Location* loc, std::vector<int> ignorar);
    Source* getSourceMaisProximo(Location* loc);

    Stop* criarStop(Location* location, Shift* shift, double arriveTime, double quantity);


    Shift* criarShift(Trailer* trailer, Driver* driver, std::vector<int> loc, double tempoInicial);
    void constructor(std::vector<Customer*>* customers, int maxInstant);
    void localSearch();
  protected:
  private:
      Solution* solAtual;
};

#endif // ILS_H
