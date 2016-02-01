#include "Shift.h"

#include <stdexcept>
#include <string>
#include <algorithm>
#include <climits>

Shift::Shift(){
    stops_.clear();
    cost_ = INFINITY;
    driver_ = NULL;
    trailer_ = NULL;
    solution_ = NULL;
    initialInstant_ = 0;
    finalInstant_ = 0;
    initialLoad_ = 0;
    remnantLoad_ = 0;
}

Shift::Shift(double cost, Driver* driver, Trailer* trailer) : cost_(cost), driver_(driver), trailer_(trailer) {
    stops_.clear();
    solution_ = NULL;
    initialInstant_ = 0;
    finalInstant_ = 0;
    initialLoad_ = 0;
    remnantLoad_ = 0;
}


Shift::~Shift(){
    for(Stop *s: stops_){
        delete s;
    }
    stops_.clear();
}

double Shift::calcCost(bool print){
    double trailerCost = trailer_->getDistanceCost();
    double driverCost = driver_->getTimeCost();
    double distance = 0;
    double time = 0;
    int maxCapacity = 0;
    int negativeQuantity = 0;
    int arrivalTime = 0;//tempo de chegada + setup time + tempo de viagem
    Base *standardBase = NULL;
    double distanceTime = 0; // guarda o tempo para o arrivalTime

    if((int)stops_.size() >= 2){
        for(int i=0; i<(int)stops_.size() - 1; i++){
            distance = InputData::getDistance(stops_.at(i)->getLocation()->getIndex(),stops_.at(i+1)->getLocation()->getIndex());
        }
        distance = InputData::getDistance(stops_.at(stops_.size()-1)->getLocation()->getIndex(),stops_.at(0)->getLocation()->getIndex());
    }
    time = finalInstant_ - initialInstant_;

    //procurando inviabilidade na solução

    for(int i=0; i<(int)stops_.size(); i++){
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

    if(print){
        if(trailerCost > 0){
            Formatter() << Penalties::toString(TRAILER_COST_MULTIPLIER) <<": "<< trailerCost<<"\n";
        }
        if(driverCost > 0){
            Formatter() << Penalties::toString(DRIVER_COST_MULTIPLIER) <<": "<< driverCost<<"\n";
        }
        if(maxCapacity > 0){
            Formatter() << Penalties::toString(TRAILER_MAX_CAPACITY) <<": "<< maxCapacity<<"\n";
        }
        if(negativeQuantity > 0){
            Formatter() << Penalties::toString(TRAILER_NON_NEGATIVE_QUANTITY) <<": "<< negativeQuantity<<"\n";
        }
        if(arrivalTime > 0){
            Formatter() << Penalties::toString(STOP_ARRIVAL_TIME) <<": "<< arrivalTime<<"\n";
        }
    }

    return cost_;

}

double Shift::getQuantityDelivered(){
    int quantity = 0;
    for(int i=0;i<stops_.size();i++){
        quantity += stops_.at(i)->getQuantity();
    }
    return quantity;
}

std::string Shift::toString(std::string tab) const {
    int i=0;
    char data[4096], info[100];

    sprintf(info,"Trailer=%02d",trailer_->getIndex());
    sprintf(data,"%sShift %22sStops:  ",tab,info);
    toString_appendStop(data,i++);

    sprintf(info,"Driver=%02d",driver_->getIndex());
    sprintf(data,"%s\n%s      %30s",data,tab,info);
    toString_appendStop(data,i++);

    sprintf(info,"Load=%.0f-%.0f",initialLoad_,remnantLoad_);
    sprintf(data,"%s\n%s      %30s",data,tab,info);
    toString_appendStop(data,i++);

    sprintf(info,"Time=%.0f-%.0f",initialInstant_,finalInstant_);
    sprintf(data,"%s\n%s      %30s",data,tab,info);
    toString_appendStop(data,i++);

    for(; i<(int)stops_.size(); i++){
        sprintf(data,"%s\n%s      %30s",data,tab," ");
        toString_appendStop(data,i++);
    }

    sprintf(data,"%s\n",data);
}
void Shift::toString_appendStop(char* data,int i) const{
    if((int)stops_.size()>i){
        sprintf(data,"%s%s",data,stops_[i]->toString().c_str());
        if((int)stops_.size()>i+1)
            sprintf(data,"%s-%.1f",data,stops_[i]->getArriveTime()+
                    stops_[i]->getLocation()->getSetupTime()+
                    InputData::getTime(stops_[i]->getLocation()->getIndex(),
                                       stops_[i+1]->getLocation()->getIndex()));
        else
            sprintf(data,"%s-%.1f   %s",data,stops_[i]->getArriveTime()+
                    stops_[i]->getLocation()->getSetupTime()+
                    InputData::getTime(stops_[i]->getLocation()->getIndex(),
                                       trailer_->getBase()->getIndex()),
                    trailer_->getBase()->getIndexStr().c_str() );
    }
}
