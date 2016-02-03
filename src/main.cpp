#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"
#include "Solution.h"


using namespace std;

int main()
{
    XMLDataLoader loader;
    loader.loadInputData("Instance_V_1.1.xml",".\\input");

    Solution* solution = new Solution();
    printf("\n\n%s",solution->toString(true).c_str());
    solution->calcCost(true);
    printf("\n\nCost = %f\n",solution->getCost());

}
