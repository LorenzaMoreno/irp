#include "XMLDataLoader.h"
#include "InputData.h"
#include "Location.h"
#include "Base.h"
#include "Customer.h"
#include "Driver.h"
#include "Trailer.h"
#include "TimeWindow.h"

#include <vector>
#include <map>
#include <math.h>

XMLDataLoader::XMLDataLoader()
{
    //ctor
}

XMLDataLoader::~XMLDataLoader()
{
    //dtor
}

void XMLDataLoader::LoadInputData()
{
    CMarkup xml;
//    if( !xml.Load(/*filename*/)) )
//        printf("\nErro ao abrir o arquivo: %s\n\n",MCD_2PCSZ(xml.GetResult())); //Abre documento

    xml.FindElem();     //Encontra raiz do documento xml
    xml.IntoElem();
/*
    LoadDrivers(xml);
    LoadTrailers(xml);
    LoadBases(xml);
    LoadSources(xml);
	LoadCustomers(xml);
	LoadTimeMatrix(xml);
	LoadDistanceMatrix(xml);*/
}
/* exemplo
void DataLoader::LoadParameters(CMarkup xml)
{
    Parameters* parameters = InputData::GetInstance()->GetParameters();

    xml.FindElem("Parametros");
    xml.IntoElem();

    xml.FindElem("PotenciaBase");
    parameters->SetBasePower(atof(MCD_2PCSZ(xml.GetData())));
    xml.ResetMainPos();

	if (xml.FindElem("ToleranciaCorte")) {
		parameters->SetTolerance(atof(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();
	}

	if (xml.FindElem("PeriodoPlanejamento")) {
		parameters->SetPlanningPeriod(atoi(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();
	}

	if (xml.FindElem("Custo")) {
		parameters->SetCost(atof(MCD_2PCSZ(xml.GetData())));
		xml.ResetMainPos();
	}

    xml.OutOfElem();
    xml.ResetMainPos();
}
*/
