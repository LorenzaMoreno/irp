#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"
#include "Solution.h"
#include <stdio.h>


using namespace std;
int main()
{
    XMLDataLoader loader;
//    loader.loadInputData("Instance_V_1.1.xml",".\\input");
    loader.loadInputData("Instance_V_1.1.xml","./input");

    Solution* solution = new Solution();
    printf("\n");
    solution->calcInitialSafetyLevelInst(InputData::getCustomers(), 120);
    printf("dividir customers por caminhao\n");
//    printf("\n\n%s",solution->toString(true).c_str());
//    solution->calcCost(true);
//    printf("\n\nCost = %f\n",solution->getCost());

}
