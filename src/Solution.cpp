#include "Solution.h"
#include "Location.h"
#include "Trailer.h"
#include <cmath>
#include <cstdlib>
#include <utility>
#include <stdio.h>
#include <stdexcept>
#include <algorithm>
#include "ILS.h"

Solution::Solution(){
    reset();
}

Solution::Solution(double cost, double infeasibilityCost):
                cost_(cost), infeasibilityCost_(infeasibilityCost){
}

Solution::~Solution(){
    driverInst_.clear();
    trailerInst_.clear();
    stockLevelInst_.clear();
    safetyLevelInst_.clear();
    trailersShifts_.clear();
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

    for(std::vector<Shift*> a : trailersShifts_){
        a.clear();
    }
    trailerInst_.clear();

    infeasibilityCost_=-1;
    cost_=-1;
}

void Solution::reset(){
    int i,j;
    int numInst = InputData::getNumInst();

    clear();
    driverInst_.resize(InputData::getDrivers()->size());
    for(i=0; i < (int)InputData::getDrivers()->size(); i++){
        driverInst_.at(i).resize(InputData::getNumInst());
        for(j=0; j < (int)driverInst_[i].size(); j++){
            driverInst_.at(i).at(j).clear();
//            driverInst_.at(i).at(j).resize(1,NULL);
        }
    }

    trailerInst_.resize(InputData::getTrailers()->size());
    for(i=0; i < (int)InputData::getTrailers()->size(); i++){
        trailerInst_.at(i).resize(InputData::getNumInst());
        for(j=0; j < (int)trailerInst_.at(i).size(); j++){
            trailerInst_.at(i).at(j).clear();
//            trailerInst_.at(i).at(j).resize(1,NULL);
        }
    }

    safetyLevelInst_.clear();
    stockLevelInst_.resize(InputData::getLocations()->size());
    i=0;
    bool safetyLevelReached = false;
    for(Location* loc : *(InputData::getLocations())){
        switch(loc->getType()){
            case Location::BASE:{
                stockLevelInst_[i].resize(InputData::getNumInst(),INFINITY);
            }break;
            case Location::SOURCE:{
                stockLevelInst_[i].resize(InputData::getNumInst(),INFINITY);
            }break;
            case Location::CUSTOMER:{
                Customer* c=(Customer*)loc;
                stockLevelInst_[i].resize(InputData::getNumInst(),c->getInitialQuantity());
                for(unsigned int j=1;j< (int)c->getForecast()->size();j++){
                    stockLevelInst_[i][j]=stockLevelInst_[i][j-1]-(*(c->getForecast()))[j];
                    if(stockLevelInst_[i][j]<0)
                        stockLevelInst_[i][j] = 0.0;
                    if(!safetyLevelReached && stockLevelInst_[i][j]<c->getSafetyLevel()){
                        safetyLevelReached = true;
                        safetyLevelInst_.insert(std::make_pair(j,c));
                    }
                }
            }break;
        }
        i++;
    }

    locationInstStop_.resize(InputData::getLocations()->size());
    for( i=0; i<(int)locationInstStop_.size(); i++ ){
        locationInstStop_[i].resize(InputData::getNumInst());
        for(j=0; j < (int)locationInstStop_[i].size(); j++)
            locationInstStop_[i][j].resize(2, NULL);
    }

    trailersShifts_.resize(InputData::getTrailers()->size());
}

int Solution::checkShift(Shift* shift, double* costDiff){
    double cost = 0;

//    Shift* shift;
    int time = shift->getFinalInstant() - shift->getInitialInstant();
    int distance = 0;

    if(!shift->getDriver()->canDrive(shift->getTrailer()))
        return Penalty::TRAILER_DRIVER_COMPATIBILITY;

    if(shift->getStop()->size() == 0)
        return Penalty::NO_PENALTIES;

    int stopsSize = shift->getStop()->size();
    for(int i = 0; i < stopsSize; i++){ // for every stop on shift
        // colisão dentro do shift
        Stop* stop = shift->getStop()->at(i);
        printf("arriveTime %d\n", stop->getArriveTime());
        double endStop;
        if(i < stopsSize - 1){ // if can have collision
            Stop* s = shift->getStop()->at(i + 1);
            endStop = stop->getArriveTime() + InputData::getInstance()->getTime(stop->getLocation()->getIndex(),
                                                                                       s->getLocation()->getIndex());
            if(endStop > s->getArriveTime())
                return Penalty::STOP_ARRIVAL_TIME;
        }
        else{
            endStop = stop->getArriveTime() + InputData::getInstance()->getTime(stop->getLocation()->getIndex(),
                                                                                       shift->getTrailer()->getBase()->getIndex());
        }

        // colisão fora do shift
        for(int t = stop->getArriveTime(); t < endStop; t++){
            if(!getLocationInstStop()->at(stop->getLocation()->getIndex()).at(t).empty())
                return Penalty::STOP_ARRIVAL_TIME;
            if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(t).empty())
                return Penalty::TRAILER_SHIFTS_OVERLAP;
            if(!getDriverInst()->at(shift->getDriver()->getIndex()).at(t).empty())
                return Penalty::DRIVER_INTERSHIFT_DURATION;
        }
    }


//    int i;
//    std::vector<Stop*>::iterator stopIt = shift->getStop()->begin();
//    Stop* stop=*(stopIt);
//    Stop* nextStop=*(stopIt + 1);
//
//    //retrieve trailer quantity
//    double trailerQuant = shift->getTrailer()->getInicialQuantity();
//    for(i=shift->getInitialInstant()-1; i>=0; i--){
//        if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).empty()){
//            Shift* lastShift = getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).at(0);
//            trailerQuant = lastShift->getRemnantLoad();
//        }
//    }
//
//    if(trailerQuant!=shift->getInitialLoad()){
//        return Penalty::TRAILER_INITIAL_QUANTITY;
//    }
//
//    //falta checar se na primeira hora tem um shift terminando
//    for(i= shift->getInitialInstant(); i<=shift->getFinalInstant(); ++i){
//        //check if driver is available
//        if(!getDriverInst()->at(shift->getDriver()->getIndex()).at(i).empty()){
//            return Penalty::DRIVER_INTERSHIFT_DURATION;
//        }
//        //check if trailer is available
//        if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).empty()){
//            return Penalty::TRAILER_SHIFTS_OVERLAP;
//        }
//        //check if location is availble
//        if(stop!=NULL && stop->getArriveTime() >= i && stop->getArriveTime() < i+1){
//            for(int k=i; k<i+stop->getLocation()->getSetupTime();k++){
//                if(!getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i).empty()){
//                    return Penalty::STOP_ARRIVAL_TIME;
//                }
//            }
//            trailerQuant-=stop->getQuantity();
//            if(trailerQuant<0){
//                return Penalty::TRAILER_NON_NEGATIVE_QUANTITY;
//            }else if(trailerQuant > shift->getTrailer()->getCapacity()){
//                return Penalty::TRAILER_MAX_CAPACITY;
//            }
//            if(stop->getLocation()->getType()==Location::CUSTOMER){
//                Customer* customer= (Customer*) stop->getLocation();
//                if(!customer->isTrailerAllowed(shift->getTrailer())){
//                    return Penalty::CUSTOMER_TRAILER_COMPATIBILITY;
//                }
//                //check stockLevel
//                for(int k=i;k< (int)getStockLevelInst()->size();k++){
//                    if( getStockLevelInst()->at(customer->getIndex()).at(k)+
//                        stop->getQuantity() > customer->getCapacity() ){
//                        return Penalty::CUSTOMER_MAX_TANK_CAPACITY;
//                    }
//                }
//            }else if(stop->getLocation()->getType()==Location::SOURCE){
//                Source* source= (Source*) stop->getLocation();
//                for(int k=i;k< (int)getStockLevelInst()->size();k++){
//                    if( getStockLevelInst()->at(source->getIndex()).at(k)+
//                        stop->getQuantity() < 0 ){
//                        return Penalty::SOURCE_NEGATIVE_CAPACITY;
//                    }
//                }
//                if(stop->getQuantity()>0){
//                    printf("Stop deve ter quantidade negativa se location for source\n");
//                    return Penalty::SOURCE_MAX_TANK_CAPACITY;
//                }
//            }
//
//            if(nextStop != NULL)
//                distance += InputData::getDistance(stop->getLocation()->getIndex(),nextStop->getLocation()->getIndex());
//
//            stopIt++;
//            stop= (stopIt!=shift->getStop()->end()) ? *stopIt : NULL;
//            nextStop= ((stopIt + 1) !=shift->getStop()->end()) ? *(stopIt + 1) : NULL;
//        }
//    }
//    (*costDiff) = distance * shift->getTrailer()->getDistanceCost() +
//               time * shift->getDriver()->getTimeCost();
//
//    /// FALTA O RETORNO -------------------------------------------------------------------------------------------

}

int Solution::checkStop(Stop* stop, double* costDiff){
    Shift* shift = stop->getShifts();

    Stop* sIt = shift->getStop()->at(0);
    int sItIndex = 0;
//    Stop* s = *sIt;
    double stopFinalTime = 0;
    double newCost;

    // case 0: the shift has no stops
    if(shift->getStop()->empty()){
        double endTime = InputData::getInstance()->getTime(shift->getTrailer()->getBase()->getIndex(), stop->getLocation()->getIndex()) +
                         InputData::getInstance()->getTime(stop->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex());
        for(int i = shift->getInitialInstant(); i < endTime; i++){
            if(!getDriverInst()->at(shift->getDriver()->getIndex()).at(i).empty()){
                return Penalty::DRIVER_INTERSHIFT_DURATION; //TODO: verificar se é esse mesmo
            }
            if(!getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i).empty()){
                return Penalty::TRAILER_SHIFTS_OVERLAP; //TODO: verificar se é esse mesmo
            }
            for(Stop* s : getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i)){
                if(s->getArriveTime() < stop->getArriveTime()){ // sLocation starts before stop
                    if(s->getArriveTime() + stop->getLocation()->getSetupTime() > stop->getArriveTime() && //s starts before stop
                       s->getArriveTime() + stop->getLocation()->getSetupTime() < stop->getArriveTime() +  //and ends before stop
                       s->getLocation()->getSetupTime()){
                        return Penalty::DRIVER_INTERSHIFT_DURATION;
                    }
                }
                else{
                    if(stop->getArriveTime() + stop->getLocation()->getSetupTime() > s->getArriveTime() && //stop starts before s
                       stop->getArriveTime() + stop->getLocation()->getSetupTime() < s->getArriveTime() +  //and ends before s
                       stop->getLocation()->getSetupTime()){ // s ends after stop start
                        return Penalty::DRIVER_INTERSHIFT_DURATION;
                    }
                }
            }
        }
        double totalDistance = InputData::getInstance()->getDistance(shift->getTrailer()->getBase()->getIndex(), stop->getLocation()->getIndex()) +
                               InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex());
        double totalTime = endTime - shift->getInitialInstant();

        *costDiff = totalTime * shift->getDriver()->getTimeCost() + totalDistance * shift->getTrailer()->getDistanceCost();

        return Penalty::NO_PENALTIES;
    }

    while(sItIndex < shift->getStop()->size() && shift->getStop()->at(sItIndex)->getArriveTime() < stop->getArriveTime())
        sItIndex++;

    if(sItIndex == shift->getStop()->size())
        sItIndex--;

    sIt = shift->getStop()->at(sItIndex);

    //case 1: the stop is the first one on the shift
    if(sItIndex == 0){
        double endTime = shift->getFinalInstant() -
        /*base to after*/ InputData::getInstance()->getTime(shift->getTrailer()->getBase()->getIndex(), sIt->getLocation()->getIndex()) +
        /*base to new  */ InputData::getInstance()->getTime(shift->getTrailer()->getBase()->getIndex(), stop->getLocation()->getIndex()) +
        /*new to after */ InputData::getInstance()->getTime(stop->getLocation()->getIndex(), sIt->getLocation()->getIndex());

//        if(tempo do final até endtime o motorista estourar tempo)
        if((endTime + shift->getFinalInstant() - shift->getInitialInstant()) > shift->getDriver()->getMaxDriving())
            return Penalty::DRIVER_MAX_DRIVING_TIME;

//        if(tempo do final até endtime o motorista estiver ocupado)
//        if(tempo do final até endtime o caminhão estiver ocupado)
        for(int i = stop->getArriveTime(); i < endTime; i++){
            for(Shift* sh : getDriverInst()->at(shift->getDriver()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::DRIVER_INTERSHIFT_DURATION;
                }
            }
            for(Shift* sh : getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::TRAILER_SHIFTS_OVERLAP;
                }
            }
        }

//        if(tempo do final até endtime location estiver livre)
        double arrival = stop->getArriveTime();
        double setup = stop->getLocation()->getSetupTime();
        double finalTime = arrival + setup;
        for(int i = arrival; i < arrival + setup; i++){
            for(Stop* s : getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i)){
                if(s->getArriveTime() < arrival){ // s arrives before stop
                    if(s->getArriveTime() + setup > arrival &&  // s ends after stop begins
                       s->getArriveTime() + setup < endTime){   // and before stop ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
                else{ // s arrives after or at the same time as stop
                    if(finalTime > s->getArriveTime() &&        // stop ends after s begins
                       finalTime < s->getArriveTime() + setup){ // and before s ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
            }
        }

        double totalDistance = InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), sIt->getLocation()->getIndex()) +
                               InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex()) -
                               InputData::getInstance()->getDistance(shift->getTrailer()->getBase()->getIndex(), sIt->getLocation()->getIndex());
        double totalTime = endTime - shift->getFinalInstant();

        *costDiff = totalTime * shift->getDriver()->getTimeCost() + totalDistance * shift->getTrailer()->getDistanceCost();

        return Penalty::NO_PENALTIES;
    }

    // case 2: the stop is the last one on the shift
    if(sItIndex == (shift->getStop()->size() - 1)){
        Stop* sItBefore = shift->getStop()->at(sItIndex - 1);
        double endTime = shift->getFinalInstant() -
        /*old back to base*/ InputData::getInstance()->getTime(sItBefore->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex()) +
        /*last to new     */ InputData::getInstance()->getTime(sItBefore->getLocation()->getIndex(), (sIt)->getLocation()->getIndex()) +
        /*new to base     */ InputData::getInstance()->getTime((sIt)->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex());

//        if(tempo do final até endtime o motorista estourar tempo)
        if((endTime + shift->getFinalInstant() - shift->getInitialInstant()) > shift->getDriver()->getMaxDriving())
            return Penalty::DRIVER_MAX_DRIVING_TIME;

//        if(tempo do final até endtime o motorista estiver ocupado)
//        if(tempo do final até endtime o caminhão estiver ocupado)
        for(int i = (sIt)->getArriveTime(); i < endTime; i++){
            for(Shift* sh : getDriverInst()->at(shift->getDriver()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::DRIVER_INTERSHIFT_DURATION;
                }
            }
            for(Shift* sh : getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::TRAILER_SHIFTS_OVERLAP;
                }
            }
        }

//        if(tempo do final até endtime location estiver livre)
        double arrival = stop->getArriveTime();
        double setup = stop->getLocation()->getSetupTime();
        double finalTime = arrival + setup;
        for(int i = arrival; i < arrival + setup; i++){
            for(Stop* s : getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i)){
                if(s->getArriveTime() < arrival){ // s arrives before stop
                    if(s->getArriveTime() + setup > arrival &&  // s ends after stop begins
                       s->getArriveTime() + setup < endTime){   // and before stop ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
                else{ // s arrives after or at the same time as stop
                    if(finalTime > s->getArriveTime() &&        // stop ends after s begins
                       finalTime < s->getArriveTime() + setup){ // and before s ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
            }
        }

        double totalDistance = InputData::getInstance()->getDistance(sItBefore->getLocation()->getIndex(), stop->getLocation()->getIndex()) +
                               InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex()) -
                               InputData::getInstance()->getDistance(sItBefore->getLocation()->getIndex(), shift->getTrailer()->getBase()->getIndex());
        double totalTime = endTime - shift->getFinalInstant();

        *costDiff = totalTime * shift->getDriver()->getTimeCost() + totalDistance * shift->getTrailer()->getDistanceCost();

        return Penalty::NO_PENALTIES;
    }

    //case 3: the stop is between other stops
    else{
        // sIt is now the stop after the stop we want to check, so, sIt - 1 is the stop before
        Stop* before;
        Stop* after;
        double endTime;
        double totalDistance;

        if(sItIndex == 0){
            after = sIt;
            endTime = shift->getFinalInstant() -
            /*base to after*/ InputData::getInstance()->getTime(shift->getTrailer()->getBase()->getIndex(), after->getLocation()->getIndex()) +
            /*base to stop */ InputData::getInstance()->getTime(shift->getTrailer()->getBase()->getIndex(), stop->getLocation()->getIndex()) +
            /*stop to after*/ InputData::getInstance()->getTime(stop->getLocation()->getIndex(), after->getLocation()->getIndex());

            totalDistance = InputData::getInstance()->getDistance(shift->getTrailer()->getBase()->getIndex(), stop->getLocation()->getIndex()) +
                               InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), after->getLocation()->getIndex()) -
                               InputData::getInstance()->getDistance(shift->getTrailer()->getBase()->getIndex(), after->getLocation()->getIndex());
        }
        else{
            before = shift->getStop()->at(sItIndex - 1);;
            after = sIt;

            endTime = shift->getFinalInstant() -
            /*before to after*/ InputData::getInstance()->getTime(before->getLocation()->getIndex(), after->getLocation()->getIndex()) +
            /*before to stop */ InputData::getInstance()->getTime(before->getLocation()->getIndex(), stop->getLocation()->getIndex()) +
            /*stop to after  */ InputData::getInstance()->getTime(stop->getLocation()->getIndex(), after->getLocation()->getIndex());

            totalDistance = InputData::getInstance()->getDistance(before->getLocation()->getIndex(), stop->getLocation()->getIndex()) +
                               InputData::getInstance()->getDistance(stop->getLocation()->getIndex(), after->getLocation()->getIndex()) -
                               InputData::getInstance()->getDistance(before->getLocation()->getIndex(), after->getLocation()->getIndex());
        }

//        if(tempo do final até endtime o motorista estourar tempo)
        if((endTime + shift->getFinalInstant() - shift->getInitialInstant()) > shift->getDriver()->getMaxDriving())
            return Penalty::DRIVER_MAX_DRIVING_TIME;

//        if(tempo do final até endtime o motorista estiver ocupado)
//        if(tempo do final até endtime o caminhão estiver ocupado)
        for(int i = stop->getArriveTime(); i < endTime; i++){
            for(Shift* sh : getDriverInst()->at(shift->getDriver()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::DRIVER_INTERSHIFT_DURATION;
                }
            }
            for(Shift* sh : getTrailerInst()->at(shift->getTrailer()->getIndex()).at(i)){
                for(Stop* s : *sh->getStop()){
                    if(s->getShifts() != shift)
                        return Penalty::TRAILER_SHIFTS_OVERLAP;
                }
            }
        }

//        if(tempo do final até endtime location estiver livre)
        double arrival = stop->getArriveTime();
        double setup = stop->getLocation()->getSetupTime();
        double finalTime = arrival + setup;
        for(int i = arrival; i < finalTime; i++){
            for(Stop* s : getLocationInstStop()->at(stop->getLocation()->getIndex()).at(i)){
                if(s->getArriveTime() < arrival){ // s arrives before stop
                    if(s->getArriveTime() + setup > arrival &&  // s ends after stop begins
                       s->getArriveTime() + setup < endTime){   // and before stop ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
                else{ // s arrives after or at the same time as stop
                    if(finalTime > s->getArriveTime() &&        // stop ends after s begins
                       finalTime < s->getArriveTime() + setup){ // and before s ends
                        return Penalty::STOP_ARRIVAL_TIME;
                    }
                }
            }
        }

        double totalTime = endTime - shift->getFinalInstant();

        *costDiff = totalTime * shift->getDriver()->getTimeCost() + totalDistance * shift->getTrailer()->getDistanceCost();

        return Penalty::NO_PENALTIES;
    }
}
void Solution::insertShift(Shift* shift){
    //Hour interval
    int iniHour_ = (shift->getInitialInstant());//Initial hour of the shift
    int finalHour_ = (shift->getFinalInstant());//Final hour of the shift
    //driverInst_
    int driverIndex_ = shift->getDriver()->getIndex();//Get the shift's driver index
    for(int i=iniHour_;i<=finalHour_;i++){
//        printf("%d",driverInst_[driverIndex_].size());
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

    //trailerInst_
    int trailerIndex_ = shift->getTrailer()->getIndex();
        /**------------------------------------------------------------------------------------**/
        //checking for discrepancies in the neighbors ( shifts )
        float netNeighborTank_=0;
        int next_=-1;//The position in the vector of the shift that sucedes this one
        int before_=-1;//The position in the vector of the shift that precedes this one
        //Checking for neighbor to the left ( after the shift )
        for(int i=finalHour_;i<=trailerInst_[trailerIndex_].size();i++){
            next_=-1;
            for(int j=0;j<trailerInst_[trailerIndex_][i].size();j++){//For every hour on the trailer's list beyond the end of THIS shift
                if(next_==-1 && trailerInst_[trailerIndex_][i].at(j)!=shift){
                    next_=0;
                }else if((trailerInst_[trailerIndex_][i].at(next_)->getInitialInstant()>
                         trailerInst_[trailerIndex_][i].at(j)->getInitialInstant()) &&
                         (trailerInst_[trailerIndex_][i].at(j)!=shift)){
                    next_ =j;
                }
            }
            if(next_!=-1){//If next_ is not -1, then we have our neighbor
                netNeighborTank_+=(trailerInst_[trailerIndex_][i].at(next_)->getInitialLoad());
                break;
            }
        }
        for(int i=iniHour_;i>=0;i--){
            before_=-1;
            for(int j=0;j<trailerInst_[trailerIndex_][i].size();j++){//For every hour on the trailer's list beyond the end of THIS shift
                if(before_==-1  && trailerInst_[trailerIndex_][i].at(j)!=shift){
                    before_=0;
                }else if((trailerInst_[trailerIndex_][i].at(before_)->getFinalInstant()<
                         trailerInst_[trailerIndex_][i].at(j)->getFinalInstant()) &&
                         (trailerInst_[trailerIndex_][i].at(j)!=shift)){
                    before_ =j;
                }
            }
            if(before_!=-1){//If before_ is not -1, then we have our neighbor
                netNeighborTank_-=(trailerInst_[trailerIndex_][i].at(before_)->getRemnantLoad());
                break;
            }
        }
        if (netNeighborTank_!=0){
            //ERROR, CAN'T REMOVE THE SHIFT
            throw std::runtime_error(Formatter() << "Error: " << "Incompatibility on trailer cargo while removing shift.\n Be sure that the trailer levels are the same ( from the shift that comes before and the one that comes after).");
        }
        /**------------------------------------------------------------------------------------**/

    for(int i=iniHour_;i<=finalHour_;i++){
        for(int j=0;j<trailerInst_[trailerIndex_][i].size();j++){//For every hour on the trailer's list
            if(trailerInst_[trailerIndex_][i].at(j)==shift){//Checking to see if the shift is present on that position
                trailerInst_[trailerIndex_][i].erase(trailerInst_[trailerIndex_][i].begin()+j);//If it is, remove it.
            }
        }
    }

    //driverInst_
    int driverIndex_ = shift->getDriver()->getIndex();
    for(int i=iniHour_;i<=finalHour_;i++){
        for(int j=0;j<driverInst_[driverIndex_][i].size();j++){//For every hour on the driver's list
            if(driverInst_[driverIndex_][i].at(j)==shift){//Checking to see if the shift is present on that position
                driverInst_[driverIndex_][i].erase(driverInst_[driverIndex_][i].begin()+j);//If it is, remove it.
            }
        }
    }

    //locationInstStop_
    for(int j=0;j<shift->getStop()->size();j++){//j = stop
        int locationIndex_ = shift->getStop()->at(j)->getLocation()->getIndex();//Getting each location index

        double iniHourStop_ = shift->getStop()->at(j)->getArriveTime();//initial hour of arrival at location
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
    calcCost();
    /**

    DONE             -> (X)VERIFICAR OS TANQUES DOS SHIFTS ANTERIOES E POSTERIORES E TRHOW EXCEPTION SE HOUVER ( NÃO AVERA REMOVEÇÃO )
                        (x)MODIFICAR OS ESTOQUES DE TODOS OS STOPS
                        (x)RETORNAR OS NOVOS CUSTO
    **/

}
/**
    Stop Sorter
**/
bool stopSorter (Stop* i, Stop* j){
    return (i->getArriveTime() < j->getArriveTime());
}

void Solution::insertStopInShift(Shift* shift, Stop* stop){
    shift->getStop()->push_back(stop);//Adding the Stop on the vector
    std::sort(shift->getStop()->begin(),shift->getStop()->end(),stopSorter);//Sorting the stops

    if( instanceof<Customer>(stop->getLocation())){
        for(int i = stop->getArriveTime();i<stockLevelInst_.size();i++){
            if(stockLevelInst_[stop->getLocation()->getIndex()][i] > 0 || i==0)
                stockLevelInst_[stop->getLocation()->getIndex()][i] += stop->getQuantity();
            else if(i!=0){
                double val = stockLevelInst_[stop->getLocation()->getIndex()][i-1] - ((Customer*)stop->getLocation())->getForecast()->at(i);
                val += stop->getQuantity();
                if(val >0)
                    stockLevelInst_[stop->getLocation()->getIndex()][i] = val;
                else
                    break;
            }
        }
    }
    calcCost();
}

void Solution::removeStopFromShift(Shift* shift, Stop* stop){
    //finding the stop on the shift ( index )
    int index_ = 0;
    for(int i=0;shift->getStop()->size();i++){
        if(shift->getStop()->at(i) == stop){
            index_ = i;
            break;
        }
    }
    //If
    if( instanceof<Customer>(stop->getLocation())){
        for(int j = shift->getInitialInstant();j<trailerInst_[shift->getTrailer()->getIndex()].size();j++){
                for(int i=0;i<trailerInst_[shift->getTrailer()->getIndex()][j].size();i++){
                    if(trailerInst_[shift->getTrailer()->getIndex()][j][i]!=shift){
                    //-------------------------------------------------
                        for(int k = 0;k<trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->size();k++){
                            if(instanceof<Source>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                                break;
                            }else if (instanceof<Customer>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                                stockLevelInst_[trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation()->getIndex()][j] +=
                                trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getQuantity();
                            }
                        }
                    }
                }
        }
    }else if(instanceof<Source>(stop->getLocation())){
        double tank_ = shift->getInitialLoad();
        //Getting the value of the trailer tank in the instant that the stop happens - OK
        for(int i = 0;i<shift->getStop()->size();i++){
            if(shift->getStop()->at(i) == stop){
                break;
            }
            tank_ -= shift->getStop()->at(i)->getQuantity();
        }
        // Verify if the shift is correct by looking at the trailer cargo level of this shift - OK

        for(int i = index_+1;i<shift->getStop()->size();i++){
            if(instanceof<Source>(shift->getStop()->at(i)->getLocation())){
                break;
            }else if (instanceof<Customer>(shift->getStop()->at(i)->getLocation())){
                if(tank_ + shift->getStop()->at(i)->getQuantity() <0){
                    throw std::runtime_error(Formatter() << "Error: " << "Invalid remove of a stop. Non-legal trailer tank cargo ( tried to give more than it had ).");
                }
                tank_ += shift->getStop()->at(i)->getQuantity();
            }
        }
        // Verify if the shift is correct by looking at the trailer cargo level in every shift after this one
        /**TODO**/
        bool flag = false;
        for(int j = shift->getInitialInstant();j<trailerInst_[shift->getTrailer()->getIndex()].size();j++){
            if (flag)break;
            for(int i=0;i<trailerInst_[shift->getTrailer()->getIndex()][j].size();i++){
                if(flag)break;
                if(trailerInst_[shift->getTrailer()->getIndex()][j][i]!=shift){
                //-------------------------------------------------
                    for(int k = 0;k<trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->size();k++){
                        if(instanceof<Source>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                            flag = true;
                            break;
                        }else if (instanceof<Customer>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                            if(tank_ + trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getQuantity() <0){
                                throw std::runtime_error(Formatter() << "Error: " << "Invalid remove of a stop. Non-legal trailer tank cargo ( tried to give more than it had ).");
                            }
                            tank_ += trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getQuantity();
                        }
                    }
                }
                //-------------------------------------------------
            }
        }

        for(int i = index_+1;i<shift->getStop()->size();i++){
            if(instanceof<Source>(shift->getStop()->at(i)->getLocation())){
                break;
            }else if (instanceof<Customer>(shift->getStop()->at(i)->getLocation())){
                if(tank_ - shift->getStop()->at(i)->getQuantity() <0){
                    throw std::runtime_error(Formatter() << "Error: " << "Invalid remove of a stop. Non-legal trailer tank cargo ( tried to give more than it had ).");
                }
                tank_ += shift->getStop()->at(i)->getQuantity();
            }
        }
    }
        /**TODO END**/

    for(int j = shift->getInitialInstant();j<trailerInst_[shift->getTrailer()->getIndex()].size();j++){
            for(int i=0;i<trailerInst_[shift->getTrailer()->getIndex()][j].size();i++){
                if(trailerInst_[shift->getTrailer()->getIndex()][j][i]!=shift){
                //-------------------------------------------------
                    for(int k = 0;k<trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->size();k++){
                        if(instanceof<Source>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                            break;
                        }else if (instanceof<Customer>(trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation())){
                            stockLevelInst_[trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getLocation()->getIndex()][j] -=
                            trailerInst_[shift->getTrailer()->getIndex()][j][i]->getStop()->at(k)->getQuantity();
                        }
                    }
                }
            }
    }

    //Removing the stop
    shift->getStop()->erase(shift->getStop()->begin()+index_);
    for(int i =0;locationInstStop_[stop->getLocation()->getIndex()][stop->getArriveTime()].size();i++){
        if(locationInstStop_[stop->getLocation()->getIndex()][stop->getArriveTime()].at(i) == stop){
            locationInstStop_[stop->getLocation()->getIndex()][stop->getArriveTime()].
                erase(locationInstStop_[stop->getLocation()->getIndex()][stop->getArriveTime()].begin()+i);
            break;
        }
    }
}


void Solution::calcCost(bool print){
    double totalQuantity = 0;
    double previousLoad;
    double cost  = 0;
    int maxTankCapacity = 0;
    int safetyLevel = 0;
    int runOut = 0;
    int driverInnerShiftPenalty = 0;
    int driverMaxDrivingPenalty = 0;
    int driverTimeWindowPenalty = 0;
    int sourceMaxTankPenalty = 0;
    int sourceNegativeTankPenalty = 0;
    int trailerShiftOverlapPenalty = 0;

    if( print )
        printf("\nComputing solution costs...\n");

    for(int i=0;i<(int)trailerInst_.size();i++){
        Shift* shift = NULL;
        previousLoad = InputData::getTrailers()->at(i)->getInicialQuantity();
        for(int j=0;j<(int)trailerInst_.at(i).size();j++){
            for(Shift *s: trailerInst_.at(i).at(j)){
                if(s == NULL)
                    continue;
                if(shift == NULL){
                    shift = s;
                    cost += s->calcCost(print);
                    totalQuantity += s->getQuantityDelivered();
                    if(abs(s->getInitialLoad()-previousLoad) > 0.5){
                        throw std::runtime_error(Formatter() << "Error: " <<
                                                 Penalties::toString(TRAILER_INITIAL_QUANTITY));
                    }
                    previousLoad = s->getRemnantLoad();
                }else if(shift != s){
                    cost += s->calcCost(print);
                    totalQuantity += s->getQuantityDelivered();
                    if(abs(s->getInitialLoad()-previousLoad) > 0.5){
                        throw std::runtime_error(Formatter() << "Error: " <<
                                                 Penalties::toString(TRAILER_INITIAL_QUANTITY));
                    }
                    previousLoad = s->getRemnantLoad();

                    //confere se o mesmo caminhão está sendo usado ao mesmo tempo
                    if(shift->getFinalInstant() > s->getInitialInstant()){
                        trailerShiftOverlapPenalty++;
                    }
                    shift = s;
                }
            }
        }
    }
    if( (cost_ > 0.001 || cost_ < 0.001) && totalQuantity > 0.001 ){
        cost_ = cost/totalQuantity;
        if( print )
                  printf("  %30s: %5d\n","TIME AND DISTANCE",cost_);
    }

    //checando se violou restrições
    for(Customer* c: *InputData::getCustomers()){
        for(double i: stockLevelInst_.at(c->getIndex())){
            if(c->getCapacity() < i){
                maxTankCapacity++;
            }

            else if(c->getSafetyLevel() > i){
                safetyLevel++;
                if(0.05 >= i){
                    runOut++;
                }
            }
        }
    }


    for(int i = 0;i<(int)driverInst_.size();i++){
        Driver *driver = NULL;
        Shift *previousShift = NULL;

        for(int j=0;j<(int)driverInst_.at(i).size();j++){
            for(Shift *s: driverInst_.at(i).at(j)){
                if(s == NULL)
                    continue;
                if(driver == NULL){
                    driver = s->getDriver();
                    previousShift = s;
                }else if(previousShift != s && s->getDriver() == driver){
                    if(s->getInitialInstant() - previousShift->getFinalInstant() < driver->getMinInterShift()){
                        driverInnerShiftPenalty++;
                        previousShift = s;
                    }
                    if(previousShift->getFinalInstant() - previousShift->getInitialInstant() > driver->getMinInterShift()){
                        driverMaxDrivingPenalty++;
                    }
                    //TODO corrigir o acesso a TimeWindows
                    //if(driver->getTimeWindow()->at(j)->getBegin() < s->getInitialInstant() && driver->getTimeWindow()->at(j)->getEnd() < s->getFinalInstant()){
                    //    driverTimeWindowPenalty++;
                    //}
                }
            }
        }
    }

    //checa se o nível de estoque do source está violando alguma restrição
    //como o nível de estoque é infinito nesta instancia, esta restrição não está sendo usada
    /*for(Source s: InputData::getSources()){
        for(int i=0; i<stockLevelInst_->at(s->getIndex())->size()){
            if(stockLevelInst_->at(s->getIndex())->at(i) > s->getMaxTankCapacity()){
                sourceMaxTankPenalty++;
            }
            if(stockLevelInst_->at(s->getIndex())->at(i) < 0){
                sourceNegativeTankPenalty++;
            }
        }
    }
    */

    cost_ += maxTankCapacity * Penalties::getValue(CUSTOMER_MAX_TANK_CAPACITY);
    if(print && maxTankCapacity > 0){
        printf("  %30s: %5d\n",Penalties::toString(CUSTOMER_MAX_TANK_CAPACITY).c_str(),maxTankCapacity);
    }

    cost_ += safetyLevel * Penalties::getValue(CUSTOMER_SAFETY_LEVEL);
    if(print && safetyLevel > 0){
        printf("  %30s: %5d\n",Penalties::toString(CUSTOMER_SAFETY_LEVEL).c_str(),safetyLevel);
    }

    cost_ += runOut * Penalties::getValue(CUSTOMER_RUN_OUT);
    if(print && runOut > 0){
        printf("  %30s: %5d\n",Penalties::toString(CUSTOMER_RUN_OUT).c_str(),runOut);
    }

    cost_ += driverInnerShiftPenalty * Penalties::getValue(DRIVER_INTERSHIFT_DURATION);
    if(print && driverInnerShiftPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(DRIVER_INTERSHIFT_DURATION).c_str(),driverInnerShiftPenalty);
    }

    cost_ += driverMaxDrivingPenalty * Penalties::getValue(DRIVER_MAX_DRIVING_TIME);
    if(print && driverMaxDrivingPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(DRIVER_MAX_DRIVING_TIME).c_str(),driverMaxDrivingPenalty);
    }

    cost_ += driverTimeWindowPenalty * Penalties::getValue(DRIVER_TIME_WINDOWS);
    if(print && driverTimeWindowPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(DRIVER_TIME_WINDOWS).c_str(),driverTimeWindowPenalty);
    }

    cost_ += sourceMaxTankPenalty * Penalties::getValue(SOURCE_MAX_TANK_CAPACITY);
    if(print && sourceMaxTankPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(SOURCE_MAX_TANK_CAPACITY).c_str(),sourceMaxTankPenalty);
    }

    cost_ += sourceNegativeTankPenalty * Penalties::getValue(SOURCE_NEGATIVE_CAPACITY);
    if(print && sourceNegativeTankPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(SOURCE_NEGATIVE_CAPACITY).c_str(),sourceNegativeTankPenalty);
    }

    cost_ += trailerShiftOverlapPenalty * Penalties::getValue(TRAILER_SHIFTS_OVERLAP);
    if(print && trailerShiftOverlapPenalty > 0){
        printf("  %30s: %5d\n",Penalties::toString(TRAILER_SHIFTS_OVERLAP).c_str(),trailerShiftOverlapPenalty);
    }

}

std::string Solution::toString(bool allData){
    char data[100000],desloc[20];
    std::stringstream str;

    str.clear();

    sprintf(data,"\nSolution:\n");

    if( allData ){
        sprintf(desloc,"           ");
        for(Customer* customer : *(InputData::getCustomers())){
            //Customer input data
            sprintf(data,"%s===\n"
                         "%s\n",data,customer->toString().c_str());
            //stock level
            sprintf(data,"%s%sStock %%=[",data,desloc);
            int hour = 0,numHours = stockLevelInst_.at(customer->getIndex()).size();
            int levelIndicator = 0;
            for(double d: stockLevelInst_.at(customer->getIndex())){
                if( d < 0.0001 )
                    levelIndicator = -1;
                else if( levelIndicator >= 0 && d > customer->getCapacity()-0.001)
                    levelIndicator = 1;

                if( hour % 24 == 23 ) {
                    sprintf(data,"%s%s%5.1f",data,
                            levelIndicator>0?"^^":levelIndicator<0?"__":"  ",
                            (d/customer->getCapacity())*(d<customer->getSafetyLevel()?-100:100));
                    if( (hour/24)%10 == 9 && hour < numHours-1 )
                        sprintf(data,"%s\n%s         ",data,desloc);
                    levelIndicator=0;
                }
                hour++;
            }
            sprintf(data,"%s ]\n",data);
            str << data;
            data[0] = '\0';
        }
    }

    sprintf(data,"\nSolution shifts:\n");

    for(int i=0;i<(int)trailerInst_.size();i++){
        Shift* lastShift = NULL;
        for(int j=0;j<(int)trailerInst_.at(i).size();j++){
            for(Shift *s: trailerInst_.at(i).at(j)){
                if( s == NULL )
                    continue;
                if( s == lastShift )
                    continue;

                sprintf(data,"%s===\n"
                         "%s\n",data,s->toString("   ").c_str());
                lastShift = s;
                str << data;
                data[0] = '\0';
            }
        }
    }

    str << data;
    return str.str();
}

void Solution::calcSafetyLevelInst(std::vector<Customer*>* customers, int initialInstant, int maxInstant){
        //maxInstant deve ser menor ou igual ao total de forecasts
    for(Customer* customer: *(customers)){
        double safetyLevel= customer->getSafetyLevel();
        double quantity = (stockLevelInst_.at(customer->getIndex())).at(initialInstant);
        for(int i=initialInstant ;i<maxInstant; i++){//varrer instantes
            double instConsumption=customer->getForecast()->at(i);
            quantity= quantity - instConsumption;
            if(quantity < safetyLevel){
                //estorou
                safetyLevelInst_.insert (std::make_pair(i,customer));
                printf("O customer %d atingiu o safety no instante %d\n", customer->getIndex(), i);
                break;
            }
        }
    }
}
