#include "Shift.h"

#include <stdexcept>
#include <string>
#include <algorithm>

Shift::Shift(){
    //ctor
}

Shift::Shift(double cost, Driver* driver, Trailer* trailer) : cost_(cost), driver_(driver), trailer_(trailer){
    //ctor
}


Shift::~Shift(){
    stops_.clear();
}

void Shift::calcCost(){
    double trailerCost = trailer_->getDistanceCost();
    double driverCost = driver_->getTimeCost();
    double distance = 0;
    double time = 0;
    int maxCapacity = 0;
    int negativeQuantity = 0;
    int arrivalTime = 0;//tempo de chegada + setup time + tempo de viagem
    Base *standardBase = NULL;
    double distanceTime = 0; // guarda o tempo para o arrivalTime

    if(stops_.size() >= 2){
        for(int i=0;i<stops_.size() - 1;i++){
            distance = InputData::getDistance(stops_.at(i)->getLocation()->getIndex(),stops_.at(i+1)->getLocation()->getIndex());
            time = InputData::getTime(stops_.at(i)->getLocation()->getIndex(),stops_.at(i+1)->getLocation()->getIndex());
        }
        distance = InputData::getDistance(stops_.at(stops_.size()-1)->getLocation()->getIndex(),stops_.at(0)->getLocation()->getIndex());
        time = InputData::getTime(stops_.at(stops_.size()-1)->getLocation()->getIndex(),stops_.at(0)->getLocation()->getIndex());
    }

    //procurando inviabilidade na solução

    for(int i=0;i<stops_.size();i++){


        if(stops_.at(i)->getQuantity() > trailer_->getCapacity())
            maxCapacity++;

        if(stops_.at(i)->getQuantity() < 0){
            negativeQuantity++;
        }

        if(instanceof<Customer>(stops_.at(i)->getLocation())){
            if(stops_.at(i) == stops_.at(i+1) && i < stops_.size() - 1){
                throw std::runtime_error(Formatter() << "Error: " << Penalties::toString(STOP_LOAD_LEVEL_CHANGED) << "  " << stops_.at(i));
            }

            if(std::find(((Customer*)stops_.at(i)->getLocation())->getAllowedTrailers()->begin(), ((Customer*)stops_.at(i)->getLocation())->getAllowedTrailers()->end(), trailer_) != ((Customer*)stops_.at(i)->getLocation())->getAllowedTrailers()->end()){}
            else{
                throw std::runtime_error(Formatter() << "Error: " << Penalties::toString(CUSTOMER_TRAILER_COMPATIBILITY) << "  "<< trailer_->getIndex() << "  "<< stops_.at(i)->getLocation()->getIndex());
            }
        }

        //trata o arrivalTime
        if(i == 0){
            distanceTime += InputData::getTime(stops_.at(i)->getLocation()->getIndex(), stops_.at(i+1)->getLocation()->getIndex());
        }
        if(i > 0 && i < stops_.size()-1){
            if(instanceof<Source>(stops_.at(i)->getLocation())){
                distanceTime += ((Source*)stops_.at(i)->getLocation())->getSetupTime() + InputData::getTime(stops_.at(i)->getLocation()->getIndex(), stops_.at(i+1)->getLocation()->getIndex());
            }else if(instanceof<Customer>(stops_.at(i)->getLocation())){
                distanceTime += ((Customer*)stops_.at(i)->getLocation())->getSetupTime() + InputData::getTime(stops_.at(i)->getLocation()->getIndex(), stops_.at(i+1)->getLocation()->getIndex());
            }
        }
        if(distanceTime < stops_.at(i+1)->getArriveTime()){
            arrivalTime++;
        }
    }

    if(std::find(driver_->getTrailers()->begin(), driver_->getTrailers()->end(), trailer_) != driver_->getTrailers()->end()){}
    else{
        throw std::runtime_error(Formatter() << "Error: " << Penalties::toString(TRAILER_DRIVER_COMPATIBILITY) << "  "<< this);
    }

    cost_ =  trailerCost * distance * Penalties::getValue(TRAILER_COST_MULTIPLIER) +
            driverCost * time * Penalties::getValue(DRIVER_COST_MULTIPLIER) +
            maxCapacity * Penalties::getValue(TRAILER_MAX_CAPACITY) +
            negativeQuantity * Penalties::getValue(TRAILER_NON_NEGATIVE_QUANTITY) +
            arrivalTime * Penalties::getValue(STOP_ARRIVAL_TIME);


}

double Shift::getQuantityDelivered(){
    int quantity = 0;
    for(int i=0;i<stops_.size();i++){
        quantity += stops_.at(i)->getQuantity();
    }
    return quantity;
}

void Solution::insertShift(Shift* shift){
    //Hour interval
    int iniHour_ = (shift->getInitialInstant());//Initial hour of the shift
    int finalHour_ = (shift->getFinalInstant());//Final hour of the shift

    //driverInst_
    int driverIndex_ = shift->getDriver()->getIndex();//Get the shift's driver index
    for(int i=iniHour_;i<=finalHour_;i++){
        driverInst_[driverIndex_][i].push_back(shift);//Add the shift on the driver's Instants list
    }

    //trailerInst_
    int trailerIndex_ = shift->getTrailer()->getIndex();//Get the shift's trailer index
    for(int i=iniHour_;i<=finalHour_;i++){
        trailerInst_[trailerIndex_][i].push_back(shift);//Add the shift on the trailer's Instants list
    }

    //locationInstStop_
    for(int j=0;j<shift->getStop()->size();j++){//j = stop
        int locationIndex_ = shift->getStop()->at(j)->getLocation()->getIndex();//Getting each location index

        double iniHourStop_ = shift->getStop()->at(j)->getArriveTime();//initial our of arrival at location
        double finalHourStop_ = shift->getStop()->at(j)->getArriveTime();//setting up the final hour of arrival/ hour of departure from location

        if( instanceof<Customer>(shift->getStop()->at(j)->getLocation())){
            finalHourStop_ += ((Customer*)shift->getStop()->at(j)->getLocation())->getSetupTime();
        }else if( instanceof<Source>(shift->getStop()->at(j)->getLocation()) ){
            finalHourStop_ += ((Source*)shift->getStop()->at(j)->getLocation())->getSetupTime();
        }

        for(int i=iniHourStop_;i<=finalHourStop_;i++){
            locationInstStop_[locationIndex_][i].push_back(shift->getStop()->at(j));//Adding the stop on the Instant list
        }
    }
    shift->setSolution(this);
}

void Solution::removeShift(Shift* shift){
    //Hour interval
    int iniHour_ = (shift->getInitialInstant());
    int finalHour_ = (shift->getFinalInstant());

    //driverInst_
    int driverIndex_ = shift->getDriver()->getIndex();
    for(int i=iniHour_;i<=finalHour_;i++){
        for(int j=0;j<driverInst_[driverIndex_][i].size();j++){//For every hour on the driver's list
            if(driverInst_[driverIndex_][i].at(j)==shift){//Checking to see if the shift is present on that position
                driverInst_[driverIndex_][i].erase(driverInst_[driverIndex_][i].begin()+j);//If it is, remove it.
            }
        }
    }

    //trailerInst_
    int trailerIndex_ = shift->getTrailer()->getIndex();
    for(int i=iniHour_;i<=finalHour_;i++){
        for(int j=0;j<trailerInst_[trailerIndex_][i].size();j++){//For every hour on the trailer's list
            if(trailerInst_[trailerIndex_][i].at(j)==shift){//Checking to see if the shift is present on that position
                trailerInst_[trailerIndex_][i].erase(trailerInst_[trailerIndex_][i].begin()+j);//If it is, remove it.
            }
        }
    }
    //locationInstStop_
    for(int j=0;j<shift->getStop()->size();j++){//j = stop
        int locationIndex_ = shift->getStop()->at(j)->getLocation()->getIndex();//Getting each location index

        double iniHourStop_ = shift->getStop()->at(j)->getArriveTime();//initial our of arrival at location
        double finalHourStop_ = shift->getStop()->at(j)->getArriveTime();//setting up the final hour of arrival/ hour of departure from location

        if( instanceof<Customer>(shift->getStop()->at(j)->getLocation())){
            finalHourStop_ += ((Customer*)shift->getStop()->at(j)->getLocation())->getSetupTime();

            //Updating the Stock of the customer
            for(int time=iniHourStop_;time<stockLevelInst_[locationIndex_]->size();time++){
                if(stockLevelInst_[locationIndex_][k]==0)
                    break;

                stockLevelInst_[locationIndex_][k] -= shift->getStop()->at(j)->getQuantity();

                if(stockLevelInst_[locationIndex_][k]<0){
                    stockLevelInst_[locationIndex_][k]=0;
                }
            }

        }
        }else if( instanceof<Source>(shift->getStop()->at(j)->getLocation()) ){
            finalHourStop_ += ((Source*)shift->getStop()->at(j)->getLocation())->getSetupTime();
        }




        for(int i=iniHourStop_;i<=finalHourStop_;i++){
            for(int k=0;k<locationInstStop_[locationIndex_][i].size();k++){//For every hour on the locations's list
                if(locationInstStop_[locationIndex_][i].at(k) == shift->getStop()->at(j)){

                    //Removing Stop
                    locationInstStop_[locationIndex_][i].erase(locationInstStop_[locationIndex_][i].begin()+k);//If it is, remove it.
                }
            }
        }
    }
    shift->setSolution(NULL);
    /**

    TODO             -> VERIFICAR OS SHIFTS VIZINHOS DO TRAILER
                        MODIFICAR OS ESTOQUES DE TODOS OS STOPS
                        RETORNAR OS NOVO CUSTO
    **/

}

