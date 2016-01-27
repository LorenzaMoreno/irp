#include <iostream>
#include "InputData.h"
#include "XMLDataLoader.h"

using namespace std;

int main()
{
    XMLDataLoader loader;
    loader.loadInputData("Instance_V_1.1.xml",".\\input");

   // Driver *d = new Driver(0, 10, new TimeWindow(10, 20), new Trailer());
}
