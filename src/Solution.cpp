#include "Solution.h"
#include "Location.h"
#include "Trailer.h"
#include <cmath>
#include <cstdlib>
#include <utility>
#include <stdio.h>

Solution::Solution(){
    //ctor
}

Solution::Solution(double cost, double infeasibilityCost):
                cost_(cost), infeasibilityCost_(infeasibilityCost){
}

Solution::~Solution(){
    driverInst_.clear();
    trailerInst_.clear();
    stockLevelInst_.clear();
    safetyLevelInst_.clear();
}

void Solution::clear(){
    for(std::vector<std::vector<Shift*> > c : driverInst_){
        for(std::vector<Shift*> b : c){
            b.clear();
        }
        c.clear();
    }
    driverInst_.clear();

    for(std::vector<std::vector<Shift*> > c : trailerInst_){
        for(std::vector<Shift*> b : c){
            b.clear();
        }
        c.clear();
    }
    trailerInst_.clear();

    for(std::vector<double> a : stockLevelInst_){
        a.clear();
    }
    stockLevelInst_.clear();

    for(std::vector<std::vector<Stop*> > a : locationInstStop_){
        for(std::vector<Stop*> b : a)
            b.clear();
        a.clear();
    }
    locationInstStop_.clear();

    safetyLevelInst_.clear();

    infeasibilityCost_=-1;
    cost_=-1;
}

void Solution::reset(){
    std::vector<Shift*> v;
    v.resize(2,NULL);
    v.clear();

    clear();
    driverInst_.resize(InputData::getDrivers()->size());
    for(std::vector<std::vector<Shift*> > a : driverInst_){
        a.resize(InputData::getNumInst(),v);
    }

    trailerInst_.resize(InputData::getTrailers()->size());
    for(std::vector<std::vector<Shift*> > a : trailerInst_){
        a.resize(InputData::getNumInst(),v);
    }

    safetyLevelInst_.clear();
    stockLevelInst_.resize(InputData::getLocations()->size());
    int i=0;
    bool safetyLevelReached = false;
    for(Location* loc : *(InputData::getLocations())){
        switch(loc->getType()){
            case Location::BASE:{
                stockLevelInst_[i].clear();
            }break;
            case Location::SOURCE:{
                stockLevelInst_[i].resize(InputData::getNumInst(),INFINITY);
            }break;
            case Location::CUSTOMER:{
                Customer* c=(Customer*)loc;
                stockLevelInst_[i].resize(InputData::getNumInst(),c->getInitialQuantity());
                for(unsigned int j=1;j< c->getForecast()->size();j++){
                    stockLevelInst_[i][j]=stockLevelInst_[i][j-1]-(*(c->getForecast()))[j];
                    if(!safetyLevelReached && stockLevelInst_[i][j]<c->getSafetyLevel()){
                        safetyLevelReached = true;
                        safetyLevelInst_.insert(std::make_pair(j,c));
                    }
                }
            }break;
        }
        i++;
    }

    std::vector<Stop*> s;
    s.resize(2,NULL);
    s.clear();
    locationInstStop_.resize(InputData::getLocations()->size());
    for(std::vector<std::vector<Stop*> > a : locationInstStop_){
        a.resize(InputData::getNumInst(),s);
    }
}

int Solution::checkShift(Shift* shift, double costDiff){
//    Shift* shift;
    int i;
    std::vector<Stop*>::iterator stopIt = shift->getStop()->begin();
    Stop* stop=*(stopIt);

    if(!shift->getDriver()->canDrive(shift->getTrailer())){
        return Penalty::TRAILER_DRIVER_COMPATIBILITY;
    }

    //retrieve trailer quantity
    double trailerQuant = shift->getTrailer()->getInicialQuantity();
    for(i=shift->getInitialInstant()-1; i>=0; i--){
        if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).empty()){
            Shift* lastShift = getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).at(0);
            trailerQuant = lastShift->getRemnantLoad();
        }
    }

    if(trailerQuant!=shift->getInitialLoad()){
        return Penalty::TRAILER_INITIAL_QUANTITY;
    }

    //falta checar se na primeira hora tem um shift terminando
    for(i= shift->getInitialInstant(); i<=shift->getFinalInstant(); ++i){
        //check if driver is available
        if(!getDriverInst()->at(shift->getDriver()->getIndex()).at(i).empty()){
            return Penalty::DRIVER_INTERSHIFT_DURATION;
        }
        //check if trailer is available
        if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).empty()){
            return Penalty::TRAILER_SHIFTS_OVERLAP;
        }
        //check if location is availble
        if(stop!=NULL && stop->getArriveTime() >= i && stop->getArriveTime() < i+1){
            for(int k=i; k<i+stop->getLocation()->getSetupTime();k++){
                if(!getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i).empty()){
                    return Penalty::STOP_ARRIVAL_TIME;
                }
            }
            trailerQuant-=stop->getQuantity();
            if(trailerQuant<0){
                return Penalty::TRAILER_NON_NEGATIVE_QUANTITY;
            }else if(trailerQuant > shift->getTrailer()->getCapacity()){
                return Penalty::TRAILER_MAX_CAPACITY;
            }
            if(stop->getLocation()->getType()==Location::CUSTOMER){
                Customer* customer= (Customer*) stop->getLocation();
                if(!customer->isTrailerAllowed(shift->getTrailer())){
                    return Penalty::CUSTOMER_TRAILER_COMPATIBILITY;
                }
                //check stockLevel
                for(int k=i;k< (int)getStockLevelInst()->size();k++){
                    if( getStockLevelInst()->at(customer->getIndex()).at(k)+
                        stop->getQuantity() > customer->getCapacity() ){
                        return Penalty::CUSTOMER_MAX_TANK_CAPACITY;
                    }
                }
            }else if(stop->getLocation()->getType()==Location::SOURCE){
                Source* source= (Source*) stop->getLocation();
                for(int k=i;k< (int)getStockLevelInst()->size();k++){
                    if( getStockLevelInst()->at(source->getIndex()).at(k)+
                        stop->getQuantity() < 0 ){
                        return Penalty::SOURCE_NEGATIVE_CAPACITY;
                    }
                }
                if(stop->getQuantity()>0){
                    printf("Stop deve ter quantidade negativa se location for source\n");
                    return Penalty::SOURCE_MAX_TANK_CAPACITY;
                }
            }
            stopIt++;
            stop= (stopIt!=shift->getStop()->end()) ? *stopIt : NULL;
        }
    }
}

bool Solution::checkStop(Stop* stop){
    /*
        1º Does the Shift Fits?
            Verify if there is not another Stop at the instant
        2º Conflict with another driver/trailer
            Verify if there is another Stop in the same location at the same instant
    */
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
            for(int time=iniHourStop_;time<stockLevelInst_[locationIndex_].size();time++){
                if(stockLevelInst_[locationIndex_][time]==0)
                    break;

                stockLevelInst_[locationIndex_][time] -= shift->getStop()->at(j)->getQuantity();

                if(stockLevelInst_[locationIndex_][time]<0){
                    stockLevelInst_[locationIndex_][time]=0;
                }
            }

        }
		else if( instanceof<Source>(shift->getStop()->at(j)->getLocation()) ){
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

void Solution::insertStopInShift(Shift* shift, Stop* stop){
}

void Solution::removeStopFromShift(Shift* shift, Stop* stop){
}

void Solution::calcCost(){
    double totalQuantity = 0;
    double previousLoad;
    double cost  = 0;
    int maxTankCapacity = 0;
    int safetyLevel = 0;
    int runOut = 0;

    for(int i=0;i<trailerInst_.size();i++){
        Shift* shift = NULL;
        previousLoad = InputData::getTrailers().at(i)->getInicialQuantity();
        for(int j=0;j<trailerInst_.at(i).size();j++){
            for(int k=0;k<trailerInst_.at(i).at(j).size();k++){
                if(shift == NULL || shift != trailerInst_.at(i).at(j).at(k)){
                    shift = trailerInst_.at(i).at(j).at(k);
                    cost += shift->getCost();
                    totalQuantity += shift->getQuantityDelivered();
                    if(abs(shift->getInitialLoad()-previousLoad) > 0.5){
                        throw std::runtime_error(Formatter() << "Error: " <<
                                                 Penalties::toString(TRAILER_INITIAL_QUANTITY));
                    }
                    previousLoad = shift->getRemnantLoad();
                }
            }
        }
    }
    cost_ = cost/totalQuantity;

    //checando se violou restrições
    for(Customer* c: *InputData::getCustomers()){
        for(double i: stockLevelInst_.at(c->getIndex())){
            if(c->getCapacity() < i){
                maxTankCapacity++;
            }

            else if(c->getSafetyLevel() > i){
                safetyLevel++;
                if(0 >= i){
                    runOut++;
                }

            }
        }
    }

    cost_ += maxTankCapacity * Penalties::getValue(CUSTOMER_MAX_TANK_CAPACITY) +
            safetyLevel * Penalties::getValue(CUSTOMER_SAFETY_LEVEL) +
            runOut * Penalties::getValue(CUSTOMER_RUN_OUT);

}
