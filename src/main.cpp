#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"
#include "Solution.h"
#include "ILS.h"
#include <stdio.h>

using namespace std;
int main(){
    XMLDataLoader loader;
    loader.loadInputData("Instance_V_1.1.xml","./input");
<<<<<<< HEAD
    printf("\n");
    Solution* solution = new Solution();
    solution->calcSafetyLevelInst(InputData::getCustomers(),0,720);
    printf("\n");
    InputData::getInstance()->calcNeighborsLocations();
    ILS* ils= new ILS(solution);
    ils->constructor(1, solution);
//    NAMALAH EH ESSE

=======
    printf("\n");
    Solution* solution = new Solution();
//    solution->calcSafetyLevelInst(InputData::getCustomers(),0,480);
    InputData::getInstance()->calcNeighborsLocations();
    ILS* ils= new ILS(solution);
    ils->constructor(20, solution);
>>>>>>> 0467ebf59680f43fc078e52abaa43e6ed2544895
}


