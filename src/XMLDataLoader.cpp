#include "XMLDataLoader.h"
#include "InputData.h"
#include "Location.h"
#include "Base.h"
#include "Customer.h"
#include "Trailer.h"
#include "TimeWindow.h"
#include "Source.h"
#include "Formatter.h"

#include <cstdio>
#include <utility>
#include <vector>
#include <map>
#include <math.h>

XMLDataLoader::XMLDataLoader()
{
    InputData* input = InputData::getInstance();
    trailerDrivers.clear();
}

XMLDataLoader::~XMLDataLoader()
{
    trailerDrivers.clear();
}

void XMLDataLoader::loadInputData(std::string file,std::string folder)
{
    std::string fileAddress = folder + "\\" + file;

    CMarkup xml;
    if( !xml.Load(fileAddress) )
        throw std::runtime_error(Formatter() << "\n\nFile not found: \""
                                             << fileAddress << "\" \n"
                                             << MCD_2PCSZ(xml.GetResult()) << "\n\n");

    xml.FindElem();     //Encontra raiz do documento xml
    xml.IntoElem();

    loadDrivers(xml);
    loadTrailers(xml);
    loadBases(xml);
    loadSources(xml);
	loadCustomers(xml);
	loadTimeAndDistanceMatrices(xml);
}

void XMLDataLoader::loadDrivers(CMarkup xml)
{
    printf("\nLoading drivers...\n");
    Driver* driver = NULL;

    xml.FindElem("drivers");
    xml.IntoElem();

	while (xml.FindElem("IRP_Roadef_Challenge_Instance_driver"))
    {
        driver = new Driver();

        xml.IntoElem();

		xml.FindElem("index");
		driver->setIndex(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("maxDrivingDuration");
		driver->setMaxDriving(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("trailer");
		trailerDrivers.insert(std::make_pair(atoi(MCD_2PCSZ(xml.GetData())),driver));
		xml.ResetMainPos();

		xml.FindElem("minInterSHIFTDURATION");
		driver->setMinInterShift(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("TimeCost");
		driver->setTimeCost(atof(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("timewindows");
		xml.IntoElem();
		while(xml.FindElem("TimeWindow")){
            TimeWindow* timeWindow = new TimeWindow();

            xml.IntoElem();

            xml.FindElem("start");
            timeWindow->setBegin(atoi(MCD_2PCSZ(xml.GetData())));
            xml.ResetMainPos();

            xml.FindElem("end");
            timeWindow->setEnd(atoi(MCD_2PCSZ(xml.GetData())));
            xml.ResetMainPos();

            xml.OutOfElem();

            driver->getTimeWindow()->push_back(timeWindow);

		}
        xml.OutOfElem();//out of timewindows
        xml.OutOfElem();//out of driver

        InputData::getDrivers()->push_back(driver);
        printf("%s\n",driver->toString().c_str());
    }
    xml.OutOfElem();//out of drivers
    xml.ResetMainPos();
}

void XMLDataLoader::loadTrailers(CMarkup xml)
{
    printf("\nLoading trailers...\n");
    Trailer* trailer = NULL;

    xml.FindElem("trailers");
    xml.IntoElem();

	while (xml.FindElem("IRP_Roadef_Challenge_Instance_Trailers"))
    {
        trailer = new Trailer();

        xml.IntoElem();

		xml.FindElem("index");
		trailer->setIndex(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("Capacity");
		trailer->setCapacity(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("InitialQuantity");
		trailer->setInitialQuantity(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("DistanceCost");
		trailer->setDistanceCost(atof(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

        std::multimap<int,Driver*>::iterator it = trailerDrivers.begin();
        for( ; it != trailerDrivers.end(); ++it ){
            if( it->first == trailer->getIndex() ){
                it->second->getTrailers()->push_back(trailer);
                trailer->getDrivers()->push_back(it->second);
            }
        }
        xml.OutOfElem();//out of trailer

        InputData::getTrailers()->push_back(trailer);
        printf("%s\n",trailer->toString().c_str());
    }
    xml.OutOfElem();//out of trailers
    xml.ResetMainPos();
}


void XMLDataLoader::loadBases(CMarkup xml)
{
    printf("\nLoading bases...\n");
    Base* base = new Base();

    xml.FindElem("bases");
    xml.IntoElem();

    xml.FindElem("index");
    base->setIndex(atoi(MCD_2PCSZ(xml.GetData())));
    xml.ResetMainPos();

    for(Driver* driver : *(InputData::getDrivers()))
        base->getDrivers()->push_back(driver);

    for(Trailer* trailer : *(InputData::getTrailers())){
        base->getTrailers()->push_back(trailer);
        trailer->setBase(base);
    }

    InputData::getBases()->push_back(base);
    InputData::getLocations()->push_back(base);
    printf("%s\n",base->toString().c_str());

    xml.OutOfElem();//out of bases
    xml.ResetMainPos();
}

void XMLDataLoader::loadSources(CMarkup xml)
{
    printf("\nLoading sources...\n");
    Source* source = NULL;

    xml.FindElem("sources");
    xml.IntoElem();

	while (xml.FindElem("IRP_Roadef_Challenge_Instance_Sources"))
    {
        source = new Source();

        xml.IntoElem();

		xml.FindElem("index");
		source->setIndex(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("setupTime");
		source->setSetupTime(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

        xml.OutOfElem();

        InputData::getSources()->push_back(source);
        InputData::getLocations()->push_back(source);
        printf("%s\n",source->toString().c_str());
    }
    xml.OutOfElem();//out of sources
    xml.ResetMainPos();
}

void XMLDataLoader::loadCustomers(CMarkup xml)
{
    printf("\nLoading customers...\n");
    Customer* customer = NULL;
    double value,totalLoad;

    xml.FindElem("customers");
    xml.IntoElem();

	while (xml.FindElem("IRP_Roadef_Challenge_Instance_Customers"))
    {
        customer = new Customer();

        xml.IntoElem();

		xml.FindElem("index");
		customer->setIndex(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("setupTime");
		customer->setSetupTime(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("Capacity");
		customer->setCapacity(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("InitialTankQuantity");
		customer->setInitialQuantity(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();

		xml.FindElem("SafetyLevel");
		customer->setSafetyLevel(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();


		xml.FindElem("allowedTrailers");
		xml.IntoElem();
		while( xml.FindElem("int")){
            Trailer* trailer = InputData::getInstance()->findTrailer(atoi(MCD_2PCSZ(xml.GetData())));
            customer->getAllowedTrailers()->push_back(trailer);
		}
		xml.OutOfElem();

        totalLoad = customer->getInitialQuantity() - customer->getSafetyLevel();
		xml.FindElem("Forecast");
		xml.IntoElem();
		while( xml.FindElem("double")){
            value = atof(MCD_2PCSZ(xml.GetData()));
            customer->getForecast()->push_back(value);
            totalLoad += value;
            customer->getTotalLoadSafety()->push_back(totalLoad>0?totalLoad:0);
		}
		xml.OutOfElem();

        xml.OutOfElem();

        InputData::getCustomers()->push_back(customer);
        InputData::getLocations()->push_back(customer);
        printf("%s\n",customer->toString().c_str());
    }
    xml.OutOfElem();//out of customers
    xml.ResetMainPos();
}

void XMLDataLoader::loadTimeAndDistanceMatrices(CMarkup xml){
    InputData* input = InputData::getInstance();

    int numLocations = (int)input->getLocations()->size();
    int orig,dest;
    int* intValues;
    double* values;

    printf("\nResizing matrices...");
    input->resizeTimeAndDistanceMatrices(numLocations);

    printf("\nLoading time matrix...");
    xml.FindElem("timeMatrices");
    xml.IntoElem();
    for(orig=0; orig<numLocations && xml.FindElem("ArrayOfInt"); orig++){
        xml.IntoElem();
        intValues = input->getTimeInMinutesFrom(orig);
        values = input->getTimeFrom(orig);
        for(dest=0; dest<numLocations && xml.FindElem("int"); dest++){
            intValues[dest] = atoi(MCD_2PCSZ(xml.GetData()));
            values[dest] = intValues[dest]/60.0;
        }
        xml.OutOfElem();
    }
    xml.OutOfElem();


    printf("\nLoading distance matrix...");
    xml.FindElem("DistMatrices");
    xml.IntoElem();
    for(orig=0; orig<numLocations && xml.FindElem("ArrayOfDouble"); orig++){
        xml.IntoElem();
        values = input->getDistancesFrom(orig);
        for(dest=0; dest<numLocations && xml.FindElem("double"); dest++)
            values[dest] = atof(MCD_2PCSZ(xml.GetData()));
        xml.OutOfElem();
    }
    xml.OutOfElem();
}
