#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"
#include "Solution.h"
#include "ILS.h"
#include <stdio.h>

using namespace std;
int main(){
    XMLDataLoader loader;
//    loader.loadInputData("Instance_V_1.1.xml",".\\input");
    loader.loadInputData("Instance_V_1.1.xml","./input");

    Solution* solution = new Solution();
    printf("\n");
    ILS* ils= new ILS(solution);
    ils->constructor(InputData::getInstance()->getCustomers(),30);

//    printf("\n\n%s",solution->toString(true).c_str());
//    solution->calcCost(true);
//    printf("\n\nCost = %f\n",solution->getCost());

}


