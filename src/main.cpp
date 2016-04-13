#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"
#include "Solution.h"
#include "ILS.h"
#include <stdio.h>
#include <time.h>

using namespace std;
int main(){
    XMLDataLoader loader;
    loader.loadInputData("Instance_V_1.1.xml","./input");
    printf("\n");

    srand(time(NULL));

    Solution* solution = new Solution();
    solution->calcSafetyLevelInst(InputData::getCustomers(),0,120);
    printf("\n");
    InputData::getInstance()->calcNeighborsLocations();
    ILS* ils= new ILS(solution);
    printf("\n");
    ils->constructor(720);
    printf("Verificacao\n");
    ils->verificarSolucao();

    printf("\n\n*****FIM*****\n");
}
