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
    double amountSupply(Customer* customer, int time, double instant, double stockAtInstant);
    Shift* criarShift(Trailer* trailer, Driver* driver, std::vector<int> locais, double tempoInicial);

//    void constructor(std::vector<Customer*>* customers, int maxInstant);
    void constructor(int maxInstant);

    void verificarSolucao();

    void localSearch();
  protected:
  private:
      Solution* solAtual;
};

#endif // ILS_H
